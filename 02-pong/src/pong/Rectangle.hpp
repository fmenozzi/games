#pragma once

#include <SFML/Graphics.hpp>

struct Rectangle
{
    sf::RectangleShape shape;

    float width()  const noexcept { return shape.getSize().x; }
    float height() const noexcept { return shape.getSize().y; }
    float left()   const noexcept { return shape.getPosition().x - width() / 2.f; }
    float right()  const noexcept { return shape.getPosition().x + width() / 2.f; }
    float top()    const noexcept { return shape.getPosition().y - height() / 2.f; }
    float bottom() const noexcept { return shape.getPosition().y + height() / 2.f; }
};