#pragma once

#include <SFML/Graphics.hpp>

struct Circle
{
    sf::CircleShape shape;

    float x()      const noexcept { return shape.getPosition().x; }
    float y()      const noexcept { return shape.getPosition().y; }
    float radius() const noexcept { return shape.getRadius(); }
    float left()   const noexcept { return x() - radius(); }
    float right()  const noexcept { return x() + radius(); }
    float top()    const noexcept { return y() - radius(); }
    float bottom() const noexcept { return y() + radius(); }
};