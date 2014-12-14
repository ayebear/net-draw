// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef PAINTTOOLS_H
#define PAINTTOOLS_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>

/*
Holds a single paint tool.
The position, color, size, and state can be changed.
A pre-set list of colors can be created.
Note: updateShape() needs to be called to set the properties of the shape object.
TODO: Move this to a separate file, rename to PaintTool
*/
struct Tool
{
    Tool();
    void updateShape();
    void changeColor(int delta);
    static void addColor(const sf::Color& color);

    enum State
    {
        Up = 0,
        Down,
        Erase
    };

    static std::vector<sf::Color> colors;

    sf::Vector2f pos;
    sf::Color color;
    unsigned size;
    int state;
    int colorId;
    bool changed;
    sf::CircleShape shape;
};

// Packet support for the Tool struct
sf::Packet& operator<<(sf::Packet& packet, const Tool& tool);
sf::Packet& operator>>(sf::Packet& packet, Tool& tool);

#endif
