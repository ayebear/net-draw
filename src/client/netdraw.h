// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef NETDRAW_H
#define NETDRAW_H

#include <SFML/Graphics.hpp>
#include "drawingboard.h"
#include "painttools.h"
#include "configfile.h"
#include "client.h"

/*
This class is the main application (handles window, input, etc.)
*/
class NetDraw
{
    public:
        NetDraw();
        void start();

    private:
        // Main state functions
        void handleEvents();
        void handleInput();
        void update();
        void draw();

        // Miscellaneous functions
        void handleMouseWheelMoved(int delta);
        void changeColor(int delta);
        void changeAlpha(int delta);
        void changeSize(int delta);
        void setupColors();
        void getSetupPackets();
        void sendToolUpdate();

        // Networking
        void handleToolUpdatePacket(sf::Packet& packet);
        void handleBoardUpdatePacket(sf::Packet& packet);
        void handleToolIdPacket(sf::Packet& packet);

        // Main objects
        sf::RenderWindow window;
        DrawingBoard board;
        std::map<int, Tool> tools;
        net::Client client;
        cfg::File config;

        // Other objects
        float dt;
        int myId;
        bool showCursor;
        bool myToolChanged; // TODO: Delete this
        bool receivedId;
        bool receivedBoard;
        bool receivedToolUpdate;
        sf::Vector2f oldMousePos;
        sf::Vector2u resolution;
};

#endif
