// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "netdraw.h"
#include "colorcode.h"

NetDraw::NetDraw():
    config("config.txt", cfg::File::AllFlags)
{
    setupColors();
    myId = tools.addTool(Tool());
    board.setSize(1024, 768);
    window.create(sf::VideoMode(1024, 768), "NetDraw v0.1 Alpha");
    showCursor = false;
    window.setMouseCursorVisible(showCursor);
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
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::MouseWheelMoved)
            handleMouseWheelMoved(event.mouseWheel.delta);
        else if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::C:
                    board.clear(); // TODO: Update this over network
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
    }
}

void NetDraw::handleInput()
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool mouseLeftDown = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool mouseRightDown = sf::Mouse::isButtonPressed(sf::Mouse::Right);

    // Update your tool
    tools[myId].pos = mousePos;

    auto oldState = tools[myId].state;
    if (mouseLeftDown)
        tools[myId].state = Tool::Down;
    else if (mouseRightDown)
        tools[myId].state = Tool::Erase;
    else
        tools[myId].state = Tool::Up;

    // Draw!
    if (mousePos != oldMousePos || oldState != tools[myId].state)
    {
        if (tools[myId].state == Tool::Down)
            board.drawLine(mousePos, mousePos, tools[myId].size, tools[myId].color);
        else if (tools[myId].state == Tool::Erase)
            board.drawLine(mousePos, mousePos, tools[myId].size, sf::Color::Black);
    }

    oldMousePos = mousePos;
}

void NetDraw::update()
{
    tools[myId].updateShape();
    network.update();
}

void NetDraw::draw()
{
    window.clear();
    window.draw(board);
    window.draw(tools);
    window.display();
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
}

void NetDraw::changeSize(int delta)
{
    tools[myId].size += delta * ceil(0.1 * tools[myId].size);
    if (tools[myId].size < 1)
        tools[myId].size = 1;
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
