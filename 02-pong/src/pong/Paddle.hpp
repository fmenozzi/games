#pragma once

#include <SFML/Graphics.hpp>

#include <pong/Entity.hpp>
#include <pong/Rectangle.hpp>
#include <pong/Manager.hpp>
#include <pong/Globals.hpp>

class Paddle : public Entity, public Rectangle
{
public:
    sf::Color color{sf::Color::Black};

    static constexpr float width{20.f}, height{120.f};
    static constexpr float speed{500.f};

    sf::Vector2f velocity;

    Paddle(float x, float y) 
    { 
        init(x,y);
    }

    void init(float x, float y)
    {
        shape.setPosition(x, y);
        shape.setSize({width, height});
        shape.setFillColor(color);
        shape.setOrigin(width / 2.f, height / 2.f);
    }

    void update(sf::Time dt, bool isPlayerPaddle, bool ballIsMovingUp)
    {
        if (isPlayerPaddle)
            processPlayerInput();
        else
            processAIInput(ballIsMovingUp);

        shape.move(velocity * dt.asSeconds());
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
    void processPlayerInput()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) and top() > 0)
            velocity.y = -speed;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) and bottom() < WIN_HEIGHT) 
            velocity.y = speed;
        else
            velocity.y = 0;
    }

    void processAIInput(bool ballIsMovingUp)
    {
        if (ballIsMovingUp and top() > 0)
            velocity.y = -speed;
        else if (not ballIsMovingUp and bottom() < WIN_HEIGHT)
            velocity.y = speed;
        else
            velocity.y = 0;
    }
};