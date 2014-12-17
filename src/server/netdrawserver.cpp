// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "netdrawserver.h"
#include "packets.h"
#include <iostream>

NetDrawServer::NetDrawServer():
    config("server.cfg")
{
    // Setup board
    board.setSize(config("width").toInt(), config("height").toInt());
    board.clear();

    // Setup server
    using namespace std::placeholders;
    server.setPacketCallback(std::bind(&NetDrawServer::handlePacket, this, _1, _2));
    server.setConnectedCallback(std::bind(&NetDrawServer::handleClientConnected, this, _1));
    server.setDisconnectedCallback(std::bind(&NetDrawServer::handleClientDisconnected, this, _1));
    server.setListeningPort(config("port").toInt());
}

NetDrawServer::~NetDrawServer()
{
}

void NetDrawServer::start()
{
    server.start();
    std::cout << "Server is running on port " << config("port").toInt() << "...\n";
    server.join();
}

void NetDrawServer::handleClientConnected(int id)
{
    std::cout << "Client " << id << " connected.\n";

    // Send tool ID
    sf::Packet packet;
    packet << sf::Int32(Packets::ToolId) << sf::Int32(id);
    server.send(packet, id);

    // Send board information and texture
    packet.clear();
    packet << sf::Int32(Packets::BoardUpdate);
    board.writeToPacket(packet);
    server.send(packet, id);
}

void NetDrawServer::handleClientDisconnected(int id)
{
    std::cout << "Client " << id << " disconnected.\n";
    tools.erase(id);

    // Let clients know that this client left
    sf::Packet packet;
    packet << sf::Int32(Packets::DeleteTool) << sf::Int32(id);
    server.sendToAll(packet, id);
}

void NetDrawServer::handlePacket(sf::Packet& packet, int id)
{
    sf::Int32 type;
    if (packet >> type)
    {
        switch (type)
        {
            case Packets::ToolUpdate:
                handleToolUpdate(packet, id);
                break;
            case Packets::ClearBoard:
                handleClearBoard(id);
                break;
            default:
                break;
        }
    }
}

void NetDrawServer::handleToolUpdate(sf::Packet& packet, int id)
{
    sf::Int32 clientId;
    if (packet >> clientId && clientId == id)
    {
        // Update the tool
        packet >> tools[id];

        // Draw the change
        board.drawTools(tools);

        // Send a packet to everyone with the update
        sf::Packet updatePacket;
        updatePacket << sf::Int32(Packets::ToolUpdate) << clientId << tools[id];
        server.sendToAll(updatePacket, id);
    }
}

void NetDrawServer::handleClearBoard(int id)
{
    board.clear();

    // Let all of the clients know to clear their boards
    sf::Packet packet;
    packet << sf::Int32(Packets::ClearBoard);
    server.sendToAll(packet, id);
}
