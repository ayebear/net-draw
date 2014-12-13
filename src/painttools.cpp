// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "painttools.h"
#include <cmath>

std::vector<sf::Color> Tool::colors;

Tool::Tool():
    pos(0, 0),
    color(sf::Color::White),
    size(16),
    state(Up),
    colorId(0)
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

PaintTools::PaintTools()
{
}

size_t PaintTools::addTool(const Tool& tool)
{
    toolList.push_back(tool);
    return (toolList.size() - 1);
}

Tool& PaintTools::operator[](size_t id)
{
    return toolList[id];
}

void PaintTools::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& tool: toolList)
        target.draw(tool.shape);
}
