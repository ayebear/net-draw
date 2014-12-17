// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

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

        void handleToolUpdate(sf::Packet& packet, int id);
        void handleClearBoard(int id);

        cfg::File config;
        net::TcpServer server;
        DrawingBoard board;
        PaintToolMap tools;
};

#endif
