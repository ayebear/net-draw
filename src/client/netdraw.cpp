// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "netdraw.h"
#include "colorcode.h"
#include "packets.h"
#include <iostream>
#include <functional>

NetDraw::NetDraw():
    config("client.cfg")
{
    setupColors();
    resolution = sf::Vector2u(1024, 768);
    windowFocus = true;
    receivedId = false;
    receivedBoard = false;
    receivedToolUpdate = false;
    myId = 0;

    // Setup network client
    using namespace std::placeholders;
    client.registerCallback(Packets::ToolUpdate, std::bind(&NetDraw::handleToolUpdatePacket, this, _1));
    client.registerCallback(Packets::ToolId, std::bind(&NetDraw::handleToolIdPacket, this, _1));
    client.registerCallback(Packets::DeleteTool, std::bind(&NetDraw::handleDeleteToolPacket, this, _1));
    client.registerCallback(Packets::BoardUpdate, std::bind(&NetDraw::handleBoardUpdatePacket, this, _1));
    client.registerCallback(Packets::ClearBoard, std::bind(&NetDraw::handleClearBoardPacket, this, _1));

    // Connect to server and setup board
    if (client.connect(net::Address(config("server"))))
        getSetupPackets();
    else
        std::cerr << "Error connecting to " << config("server") << "\n";

    // Setup window
    window.create(sf::VideoMode(resolution.x, resolution.y), "NetDraw v0.2 Beta");
    showCursor = false;
    window.setMouseCursorVisible(showCursor);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(20);
}

void NetDraw::start()
{
    sf::Clock clock;
    while (window.isOpen())
    {
        dt = clock.restart().asSeconds();
        handleEvents();
        handleInput();
        update();
        draw();
    }
}

void NetDraw::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseWheelMoved:
                handleMouseWheelMoved(event.mouseWheel.delta);
                break;

            case sf::Event::KeyPressed:
                handleKeyPressed(event.key.code);
                break;

            case sf::Event::LostFocus:
                windowFocus = false;
                break;

            case sf::Event::GainedFocus:
                windowFocus = true;
                break;

            default:
                break;
        }
    }
}

void NetDraw::handleInput()
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool mouseLeftDown = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool mouseRightDown = sf::Mouse::isButtonPressed(sf::Mouse::Right);

    // Update your tool
    auto& myTool = tools[myId];
    myTool.pos = mousePos;

    // Set the tool's state
    auto oldState = myTool.state;
    if (mouseLeftDown && windowFocus)
        myTool.state = Tool::Down;
    else if (mouseRightDown && windowFocus)
        myTool.state = Tool::Erase;
    else
        myTool.state = Tool::Up;

    // Set tool state to changed
    if (mousePos != oldMousePos || oldState != myTool.state)
        myTool.changed = true;
    oldMousePos = mousePos;
}

void NetDraw::update()
{
    tools[myId].updateShape();

    // Send updates to server
    if (tools[myId].changed)
        sendToolUpdate();

    // Have the tools draw on the screen
    board.drawTools(tools);

    client.receive();
}

void NetDraw::draw()
{
    window.clear();
    window.draw(board);
    for (auto& tool: tools)
        window.draw(tool.second.shape);
    window.display();
}

void NetDraw::handleKeyPressed(sf::Keyboard::Key keyCode)
{
    switch (keyCode)
    {
        case sf::Keyboard::C:
            clearBoard();
            break;
        case sf::Keyboard::S:
            showCursor = !showCursor;
            window.setMouseCursorVisible(showCursor);
            break;
        case sf::Keyboard::N:
            changeColor(1);
            break;
        default:
            break;
    }
}

void NetDraw::handleMouseWheelMoved(int delta)
{
    bool ctrlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                       sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
    bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
    if (ctrlPressed)
        changeColor(delta);
    else if (shiftPressed)
        changeAlpha(delta);
    else
        changeSize(delta);
}

void NetDraw::changeColor(int delta)
{
    tools[myId].changeColor(delta);
}

void NetDraw::changeAlpha(int delta)
{
    int alpha = tools[myId].color.a;
    alpha += delta * 50;
    if (alpha < 10)
        alpha = 10;
    if (alpha > 255)
        alpha = 255;
    tools[myId].color.a = alpha;
    tools[myId].changed = true;
}

void NetDraw::changeSize(int delta)
{
    tools[myId].size += delta * ceil(0.1 * tools[myId].size);
    if (tools[myId].size < 1)
        tools[myId].size = 1;
    tools[myId].changed = true;
}

void NetDraw::setupColors()
{
    for (auto& colorOption: config("colors"))
    {
        ColorCode colorCode(colorOption.toString());
        Tool::addColor(colorCode.toColor());
        colorOption = colorCode.toString();
    }
}

void NetDraw::getSetupPackets()
{
    std::cout << "Waiting for setup packets...\n";
    while (!receivedId || !receivedBoard)
    {
        client.receive();
        sf::sleep(sf::milliseconds(50));
    }
    std::cout << "Received all setup packets.\n";
}

void NetDraw::sendToolUpdate()
{
    sf::Packet packet;
    packet << sf::Int32(Packets::ToolUpdate) << sf::Int32(myId) << tools[myId];
    client.send(packet);
}

void NetDraw::clearBoard()
{
    board.clear();
    sf::Packet packet;
    packet << sf::Int32(Packets::ClearBoard);
    client.send(packet);
}

void NetDraw::handleToolUpdatePacket(sf::Packet& packet)
{
    sf::Int32 id;
    while (packet >> id)
    {
        if (!receivedToolUpdate || id != myId)
        {
            packet >> tools[id];
            if (id == myId)
            {
                std::cout << "Received tool!\n";
                receivedToolUpdate = true;
            }
        }
    }
}

void NetDraw::handleToolIdPacket(sf::Packet& packet)
{
    sf::Int32 id;
    packet >> id;
    myId = id;
    receivedId = true;
    std::cout << "Your tool ID: " << myId << "\n";
}

void NetDraw::handleDeleteToolPacket(sf::Packet& packet)
{
    sf::Int32 id;
    packet >> id;
    tools.erase(id);
    std::cout << "Client " << id << " has left.\n";
}

void NetDraw::handleBoardUpdatePacket(sf::Packet& packet)
{
    board.readFromPacket(packet);
    resolution = board.getSize();
    receivedBoard = true;
    std::cout << "Resolution from server: " << resolution.x << " x " << resolution.y << "\n";
}

void NetDraw::handleClearBoardPacket(sf::Packet& packet)
{
    board.clear();
}
