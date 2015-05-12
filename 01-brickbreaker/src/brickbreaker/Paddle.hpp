#pragma once

#include <SFML/Graphics.hpp>

#include <brickbreaker/Entity.hpp>
#include <brickbreaker/Rectangle.hpp>

class Paddle : public Entity, public Rectangle
{
public:
    const sf::Color defColor{sf::Color::Black};
    static constexpr float defWidth{120.f}, defHeight{20.f};
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
        extern unsigned int WIN_WIDTH;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0) 
            velocity.x = -defVelocity;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < WIN_WIDTH) 
            velocity.x = defVelocity;
        else 
            velocity.x = 0;  
    }
};