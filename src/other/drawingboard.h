// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef DRAWINGBOARD_H
#define DRAWINGBOARD_H

#include <SFML/Graphics.hpp>

/*
A canvas that can be "drawn" onto with various tools.
Can also be synced over a network, and saved to a file.
*/
class DrawingBoard: public sf::Drawable
{
    public:
        DrawingBoard();
        void setSize(unsigned width, unsigned height);
        void clear(sf::Color color = sf::Color::Black);
        void drawLine(sf::Vector2f start, sf::Vector2f end, unsigned thickness, sf::Color color);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        sf::RenderTexture texture;
        sf::CircleShape shape;
};

#endif
