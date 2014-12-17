// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "painttools.h"
#include "colorcode.h"
#include <cmath>
#include <iostream>

std::vector<sf::Color> Tool::colors;

Tool::Tool():
    pos(0, 0),
    color(sf::Color::White),
    size(16),
    state(Up),
    colorId(0),
    changed(false)
{
    updateShape();
}

void Tool::updateShape()
{
    shape.setPosition(pos);
    shape.setOutlineColor(color);

    // Set drawing state
    if (state == Up)
        shape.setFillColor(sf::Color::Transparent);
    else if (state == Down)
        shape.setFillColor(color);
    else if (state == Erase)
        shape.setFillColor(sf::Color::Magenta);

    shape.setRadius(size);
    shape.setOutlineThickness(ceil(size / 4.0));
    shape.setOrigin(size, size);
}

void Tool::changeColor(int delta)
{
    if (!colors.empty())
    {
        colorId += delta;
        if (colorId < 0)
            colorId = colors.size() - 1;
        colorId %= colors.size();

        // Preserve the alpha value
        auto alpha = color.a;
        color = colors[colorId];
        color.a = alpha;
        changed = true;
    }
}

void Tool::addColor(const sf::Color& color)
{
    colors.push_back(color);
}

sf::Packet& operator<<(sf::Packet& packet, const Tool& tool)
{
    return packet << tool.pos.x << tool.pos.y << tool.color
        << sf::Uint32(tool.size) << sf::Int32(tool.state);
}

sf::Packet& operator>>(sf::Packet& packet, Tool& tool)
{
    packet >> tool.pos.x >> tool.pos.y >> tool.color >> tool.size >> tool.state;
    tool.changed = true;
    tool.updateShape();
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const sf::Color& color)
{
    return packet << color.r << color.g << color.b << color.a;
}

sf::Packet& operator>>(sf::Packet& packet, sf::Color& color)
{
    return packet >> color.r >> color.g >> color.b >> color.a;
}
