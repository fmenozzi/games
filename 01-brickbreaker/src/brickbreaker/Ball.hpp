#pragma once

#include <SFML/Graphics.hpp>

#include <brickbreaker/Entity.hpp>
#include <brickbreaker/Circle.hpp>

class Ball : public Entity, public Circle
{
public:
    const sf::Color defColor{sf::Color::Black};
    static constexpr float defRadius{10.f}, defVelocity{8.f};

    sf::Vector2f velocity{-defVelocity, -defVelocity};

    Ball(float mX, float mY)
    {
        shape.setPosition(mX, mY);
        shape.setRadius(defRadius);
        shape.setFillColor(defColor);
        shape.setOrigin(defRadius, defRadius);
    }

    void update() override
    {
        shape.move(velocity);
        solveBoundCollisions();
    }

    void draw(sf::RenderWindow& mTarget) override
    { 
        mTarget.draw(shape);
    }

private:
    void solveBoundCollisions()
    {
        extern unsigned int WIN_WIDTH, WIN_HEIGHT;
        
        if (left() < 0) 
            velocity.x = defVelocity;
        else if (right() > WIN_WIDTH) 
            velocity.x = -defVelocity;

        if (top() < 0) {
            velocity.y = defVelocity;
        } else if (bottom() > WIN_HEIGHT) {
            destroyed = true;
        }
    }
};