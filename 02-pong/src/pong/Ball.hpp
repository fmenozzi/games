#pragma once

#include <SFML/Graphics.hpp>

#include <pong/Entity.hpp>
#include <pong/Circle.hpp>
#include <pong/Globals.hpp>

class Ball : public Entity, public Circle
{
public:
    sf::Color color{sf::Color::Black};

    static constexpr float radius{BALL_RADIUS};
    static constexpr float speed{BALL_SPEED};

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

    void update(sf::Time dt) override
    {
        shape.move(velocity * dt.asSeconds());
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
        if (left() < 0 or right() > WIN_WIDTH)
            destroyed = true;

        if (top() < 0)
            velocity.y = speed;
        else if (bottom() > WIN_HEIGHT)
            velocity.y = -speed;
    }

    void solveSideOfScreen()
    {
        onLeftSideOfScreen  = x() < WIN_WIDTH/2.f;
        onUpperSideOfScreen = y() < WIN_HEIGHT/2.f;
    }
};