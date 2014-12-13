// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "drawingboard.h"

DrawingBoard::DrawingBoard()
{
}

void DrawingBoard::setSize(unsigned width, unsigned height)
{
    texture.create(width, height);
}

void DrawingBoard::clear(sf::Color color)
{
    texture.clear(color);
}

void DrawingBoard::drawLine(sf::Vector2f start, sf::Vector2f end, unsigned thickness, sf::Color color)
{
    // Use a temporary shape to draw on the render texture
    shape.setRadius(thickness);
    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color::Transparent);
    shape.setOrigin(thickness, thickness);
    shape.setPosition(start);

    // TODO: Draw a full line instead of the first point by looping or something
    // Draw it to the render texture
    texture.draw(shape);
    texture.display();
}

void DrawingBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Sprite sprite(texture.getTexture());
    target.draw(sprite);
}
