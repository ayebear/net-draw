#ifndef NETDRAWSERVER_H
#define NETDRAWSERVER_H

#include "tcpserver.h"
#include "configfile.h"
#include "drawingboard.h"
#include "painttools.h"
#include <map>

class NetDrawServer
{
    public:
        NetDrawServer();
        virtual ~NetDrawServer();
        void start();

    private:
        void handleClientConnected(int id);
        void handleClientDisconnected(int id);
        void handlePacket(sf::Packet& packet, int id);

        cfg::File config;
        net::TcpServer server;
        DrawingBoard board;
        std::map<int, Tool> tools;
};

#endif
