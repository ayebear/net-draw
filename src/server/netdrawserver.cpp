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
    std::cout << "Server is running...\n";
    server.join();
}

void NetDrawServer::handleClientConnected(int id)
{
    std::cout << "Client " << id << " connected.\n";
    tools[id].updateShape();

    // Send tool ID
    sf::Packet packet;
    packet << sf::Int32(Packets::ToolId) << sf::Int32(id);
    server.send(packet, id);

    // Send board information
    packet.clear();
    packet << sf::Int32(Packets::BoardUpdate) << config("width").to<sf::Uint32>() << config("height").to<sf::Uint32>();
    server.send(packet, id);
}

void NetDrawServer::handleClientDisconnected(int id)
{
    std::cout << "Client " << id << " disconnected.\n";
    tools.erase(id);
}

void NetDrawServer::handlePacket(sf::Packet& packet, int id)
{
    //std::cout << "Client " << id << " sent a packet.\n";
    sf::Int32 type, clientId;
    if (packet >> type >> clientId && type == Packets::ToolUpdate && clientId == id)
    {
        // Update the tool
        packet >> tools[id];

        // Send a packet to everyone with the update
        sf::Packet updatePacket;
        updatePacket << type << clientId << tools[id];
        server.send(updatePacket);
    }
}
