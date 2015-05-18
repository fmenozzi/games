#pragma once

#include <SFML/Graphics.hpp>

#include <pong/Entity.hpp>
#include <pong/Circle.hpp>

class Ball : public Entity, public Circle
{
public:
    sf::Color color{sf::Color::Black};

    static constexpr float radius{10.f};
    static constexpr float speed{8.f};

    sf::Vector2f velocity;

    Ball(float x, float y)
    {
        init(x,y);
    }

    void init(float x, float y)
    {
        shape.setPosition(x, y);
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin(radius, radius);

        velocity = sf::Vector2f{-speed, -speed};
    }

    void update() override
    {
        shape.move(velocity);
        solveBoundCollisions();
        solveSideOfScreen();
    }

    void draw(sf::RenderWindow& target) override
    { 
        target.draw(shape);
    }

    float x() const noexcept override
    {
        return shape.getPosition().x;
    }

    float y() const noexcept override
    {
        return shape.getPosition().y;
    }

private:
    void solveBoundCollisions()
    {
        extern unsigned int WIN_WIDTH, WIN_HEIGHT;

        if (left() < 0 or right() > WIN_WIDTH)
            destroyed = true;

        if (top() < 0)
            velocity.y = speed;
        else if (bottom() > WIN_HEIGHT)
            velocity.y = -speed;
    }

    void solveSideOfScreen()
    {
        extern unsigned int WIN_WIDTH, WIN_HEIGHT;

        onLeftSideOfScreen  = x() < WIN_WIDTH/2.f;
        onUpperSideOfScreen = y() < WIN_HEIGHT/2.f;
    }
};