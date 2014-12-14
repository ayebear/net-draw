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
    }
}

void Tool::addColor(const sf::Color& color)
{
    colors.push_back(color);
}

sf::Packet& operator<<(sf::Packet& packet, const Tool& tool)
{
    //std::string colorStr = ColorCode(tool.color).toString();
    //std::cout << colorStr << "\n";
    //std::cout << (int)tool.color.r << ", " << (int)tool.color.g << ", " << (int)tool.color.b << ", " << (int)tool.color.a << "\n";
    return packet << tool.pos.x << tool.pos.y << tool.color.r
        << tool.color.g << tool.color.b << tool.color.a
        << sf::Uint32(tool.size) << sf::Int32(tool.state);
}

sf::Packet& operator>>(sf::Packet& packet, Tool& tool)
{
    //std::string colorStr;
    packet >> tool.pos.x >> tool.pos.y >> tool.color.r >> tool.color.g
        >> tool.color.b >> tool.color.a >> tool.size >> tool.state;
    //std::cout << colorStr << "\n";
    //tool.color = ColorCode(colorStr).toColor();
    tool.updateShape();
    return packet;
}
