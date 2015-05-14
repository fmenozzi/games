#pragma once

#include <SFML/Graphics.hpp>

struct Circle
{
    sf::CircleShape shape;

    float radius() const noexcept { return shape.getRadius(); }
    float left()   const noexcept { return shape.getPosition().x - radius(); }
    float right()  const noexcept { return shape.getPosition().x + radius(); }
    float top()    const noexcept { return shape.getPosition().y - radius(); }
    float bottom() const noexcept { return shape.getPosition().y + radius(); }
};