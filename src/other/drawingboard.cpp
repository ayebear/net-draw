// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "drawingboard.h"
#include <iostream>

DrawingBoard::DrawingBoard():
    textureSize(0, 0)
{
    shape.setPointCount(50);
}

void DrawingBoard::setSize(unsigned width, unsigned height)
{
    // Resize the texture if the size is different
    if (textureSize.x != width || textureSize.y != height)
    {
        texture.create(width, height);
        textureSize.x = width;
        textureSize.y = height;
    }
}

const sf::Vector2u& DrawingBoard::getSize() const
{
    return textureSize;
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

    /*std::cout << "Drew " << (int)color.r << "-" << (int)color.g << "-" << (int)color.b
        << "-" << (int)color.a << " @ (" << start.x << ", " << start.y << "), radius = "
        << thickness << "\n";*/
}

void DrawingBoard::drawTools(PaintToolMap& tools)
{
    for (auto& tool: tools)
    {
        if (tool.second.changed && tool.second.state != PaintTool::Up)
        {
            // Draw or erase
            sf::Color toolColor = tool.second.color;
            if (tool.second.state == PaintTool::Erase)
                toolColor = sf::Color::Black;
            drawLine(tool.second.pos, tool.second.pos, tool.second.size, toolColor);
        }
        tool.second.changed = false;
    }
}

void DrawingBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Sprite sprite(texture.getTexture());
    target.draw(sprite);
}

void DrawingBoard::writeToPacket(sf::Packet& packet) const
{
    // Copy the texture to an image
    // Warning: Does not seem to work on NVIDIA cards on Linux with SFML 2.1
    sf::Image image(texture.getTexture().copyToImage());
    auto size = image.getSize();

    // Store the size
    packet << size.x << size.y;

    // Store the pixels
    unsigned count = 0;
    for (unsigned y = 0; y < size.y; ++y)
    {
        for (unsigned x = 0; x < size.x; ++x, ++count)
            packet << image.getPixel(x, y);
    }

    std::cout << "Stored board into packet, with " << count << " pixels.\n";
}

void DrawingBoard::readFromPacket(sf::Packet& packet)
{
    // Get the size
    sf::Vector2u size;
    packet >> size.x >> size.y;

    // Create an image to store the pixels
    sf::Image image;
    image.create(size.x, size.y);

    // Extract the pixels and store them in the image
    sf::Color color;
    unsigned count = 0;
    for (unsigned y = 0; y < size.y; ++y)
    {
        for (unsigned x = 0; x < size.x; ++x)
        {
            if (packet >> color)
            {
                image.setPixel(x, y, color);
                ++count;
            }
        }
    }

    // Draw image to render texture
    sf::Texture tmpTexture;
    tmpTexture.loadFromImage(image);
    sf::Sprite sprite(tmpTexture);
    setSize(size.x, size.y);
    clear();
    texture.draw(sprite);
    texture.display();

    std::cout << "Read board from packet, with " << count << " pixels.\n";
}
