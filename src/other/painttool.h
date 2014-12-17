// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef PAINTTOOL_H
#define PAINTTOOL_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>

/*
Holds a single paint tool.
The position, color, size, and state can be changed.
A pre-set list of colors can be created.
Note: updateShape() needs to be called to set the properties of the shape object.
*/
class PaintTool: public sf::Drawable
{
public:
    PaintTool();
    void updateShape();
    void setUsername(const std::string& text);
    void changeColor(int delta);
    static void addColor(const sf::Color& color);
    static void loadFont(const std::string& filename);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    enum State
    {
        Up = 0,
        Down,
        Erase
    };

    static std::vector<sf::Color> colors;
    static sf::Font font;
    static const unsigned fontSize = 16;

    // Properties of the paint tool
    sf::Vector2f pos;
    sf::Color color;
    unsigned size;
    int state;
    std::string username;
    int colorId;
    bool changed;
    sf::CircleShape shape;
    sf::Text usernameText;
};

// Packet support for the PaintTool struct
sf::Packet& operator<<(sf::Packet& packet, const PaintTool& tool);
sf::Packet& operator>>(sf::Packet& packet, PaintTool& tool);

// Packet support for colors
sf::Packet& operator<<(sf::Packet& packet, const sf::Color& color);
sf::Packet& operator>>(sf::Packet& packet, sf::Color& color);

// A map of client IDs to paint tools
using PaintToolMap = std::map<int, PaintTool>;

#endif
