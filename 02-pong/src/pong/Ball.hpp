#pragma once

#include <SFML/Graphics.hpp>

#include <pong/Entity.hpp>
#include <pong/Circle.hpp>

class Ball : public Entity, public Circle
{
public:
    const sf::Color color{sf::Color::Black};

    static constexpr float radius{10.f};
    static constexpr float speed{8.f};

    sf::Vector2f velocity{-speed, -speed};

    Ball(float x, float y)
    {
        shape.setPosition(x, y);
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin(radius, radius);
    }

    void update() override
    {
        shape.move(velocity);
        solveBoundCollisions();
    }

    void draw(sf::RenderWindow& target) override
    { 
        target.draw(shape);
    }

private:
    void solveBoundCollisions()
    {
        extern unsigned int WIN_WIDTH, WIN_HEIGHT;

        //if (left() < 0 or right() > WIN_WIDTH)
        //    destroyed = 0;

        if (left() < 0)
            velocity.x = speed;
        else if (right() > WIN_WIDTH)
            velocity.x = -speed;

        if (top() < 0)
            velocity.y = speed;
        else if (bottom() > WIN_HEIGHT)
            velocity.y = -speed;
    }

    void solveScreenSide()
    {
        extern unsigned int WIN_WIDTH;

        onLeftSideOfScreen = x() > WIN_WIDTH / 2.f;
    }
};