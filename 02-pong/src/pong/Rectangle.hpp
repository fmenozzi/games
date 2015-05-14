#pragma once

#include <SFML/Graphics.hpp>

struct Rectangle
{
    sf::RectangleShape shape;

    float x()      const noexcept { return shape.getPosition().x; }
    float y()      const noexcept { return shape.getPosition().y; }
    float width()  const noexcept { return shape.getSize().x; }
    float height() const noexcept { return shape.getSize().y; }
    float left()   const noexcept { return x() - width() / 2.f; }
    float right()  const noexcept { return x() + width() / 2.f; }
    float top()    const noexcept { return y() - height() / 2.f; }
    float bottom() const noexcept { return y() + height() / 2.f; }
};