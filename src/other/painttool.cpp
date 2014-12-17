// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "painttool.h"
#include "colorcode.h"
#include <cmath>
#include <iostream>

std::vector<sf::Color> PaintTool::colors;
sf::Font PaintTool::font;

PaintTool::PaintTool():
    pos(0, 0),
    color(sf::Color::White),
    size(16),
    state(Up),
    colorId(0),
    changed(false)
{
    usernameText.setFont(font);
    usernameText.setCharacterSize(fontSize);
    usernameText.setColor(sf::Color::White);
    shape.setPointCount(50);
    updateShape();
}

void PaintTool::updateShape()
{
    // Set properties
    shape.setPosition(pos);
    shape.setOutlineColor(color);
    shape.setRadius(size);
    int thickness = ceil(size / 8.0);
    shape.setOutlineThickness(thickness);
    shape.setOrigin(size, size);

    // Set drawing state
    if (state == Up)
        shape.setFillColor(sf::Color::Transparent);
    else if (state == Down)
        shape.setFillColor(color);
    else if (state == Erase)
        shape.setFillColor(sf::Color::Magenta);

    // Update username text position
    usernameText.setPosition(pos.x, pos.y - size - thickness);
}

void PaintTool::setUsername(const std::string& text)
{
    if (username != text)
    {
        // Set username text
        username = text;
        usernameText.setString(username);
        auto leftPos = usernameText.findCharacterPos(0);
        auto rightPos = usernameText.findCharacterPos(-1);
        usernameText.setOrigin((rightPos.x - leftPos.x) / 2, fontSize + 6);
    }
}

void PaintTool::changeColor(int delta)
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

void PaintTool::addColor(const sf::Color& color)
{
    colors.push_back(color);
}

void PaintTool::loadFont(const std::string& filename)
{
    font.loadFromFile(filename);
}

void PaintTool::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape);
    target.draw(usernameText);
}

sf::Packet& operator<<(sf::Packet& packet, const PaintTool& tool)
{
    return packet << tool.pos.x << tool.pos.y << tool.color
        << sf::Uint32(tool.size) << sf::Int32(tool.state) << tool.username;
}

sf::Packet& operator>>(sf::Packet& packet, PaintTool& tool)
{
    std::string username;
    packet >> tool.pos.x >> tool.pos.y >> tool.color >> tool.size >> tool.state >> username;
    tool.changed = true;
    tool.setUsername(username);
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
