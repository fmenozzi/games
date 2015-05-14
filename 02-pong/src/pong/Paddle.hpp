#pragma once

#include <SFML/Graphics.hpp>

#include <pong/Entity.hpp>
#include <pong/Rectangle.hpp>

class Paddle : public Entity, public Rectangle
{
public:
    const sf::Color defColor{sf::Color::Black};
    static constexpr float defWidth{20.f}, defHeight{120.f};
    static constexpr float defVelocity{8.f};

    sf::Vector2f velocity;

    Paddle(float mX, float mY) 
    { 
        shape.setPosition(mX, mY);
        shape.setSize({defWidth, defHeight});
        shape.setFillColor(defColor);
        shape.setOrigin(defWidth / 2.f, defHeight / 2.f);
    }

    void update() override
    {
        processPlayerInput();
        shape.move(velocity);
    }

    void draw(sf::RenderWindow& mTarget) override 
    { 
        mTarget.draw(shape); 
    }

private:
    void processPlayerInput()
    {
        extern unsigned int WIN_HEIGHT;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) and top() > 0)
            velocity.y = -defVelocity;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) and bottom() < WIN_HEIGHT) 
            velocity.y = defVelocity;
        else
            velocity.y = 0;
    }
};