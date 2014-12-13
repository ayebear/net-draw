// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef PAINTTOOLS_H
#define PAINTTOOLS_H

#include <SFML/Graphics.hpp>
#include <vector>

/*
Holds a single paint tool.
The position, color, size, and state can be changed.
A pre-set list of colors can be created.
Note: updateShape() needs to be called to set the properties of the shape object.
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
    sf::CircleShape shape;
};

/*
Contains and manages a list of paint tools controlled by multiple clients.
*/
class PaintTools: public sf::Drawable
{
    public:
        PaintTools();
        size_t addTool(const Tool& tool);
        Tool& operator[](size_t id);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        std::vector<Tool> toolList;
};

#endif
