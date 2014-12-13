// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef NETDRAW_H
#define NETDRAW_H

#include <SFML/Graphics.hpp>
#include "drawingboard.h"
#include "painttools.h"
#include "networkupdater.h"
#include "configfile.h"

/*
This class is the main application (handles window, input, etc.)
*/
class NetDraw
{
    public:
        NetDraw();
        void start();

    private:
        void handleEvents();
        void handleInput();
        void update();
        void draw();

        void handleMouseWheelMoved(int delta);
        void changeColor(int delta);
        void changeAlpha(int delta);
        void changeSize(int delta);
        void setupColors();

        sf::RenderWindow window;
        float dt;
        size_t myId;
        sf::Vector2f oldMousePos;
        bool showCursor;

        // Main objects
        DrawingBoard board;
        PaintTools tools;
        NetworkUpdater network;
        cfg::File config;
};

#endif
