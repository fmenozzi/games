#pragma once

#include <SFML/Graphics.hpp>

class Brick : public Entity, public Rectangle
{
public:
    const sf::Color defColorHits1{0, 0, 0, 80};
    const sf::Color defColorHits2{0, 0, 0, 170};
    const sf::Color defColorHits3{0, 0, 0, 255};

    static constexpr float defWidth{60.f};
    static constexpr float defHeight{20.f};
    static constexpr float defVelocity{8.f};

    int requiredHits{1};

    Brick(float mX, float mY) 
    { 
        shape.setPosition(mX, mY);
        shape.setSize({defWidth, defHeight});       
        shape.setOrigin(defWidth / 2.f, defHeight / 2.f);
    }

    void update() override
    {
        // Let's alter the color of the brick depending on the required hits.
        if (requiredHits == 1) 
            shape.setFillColor(defColorHits1);
        else if (requiredHits == 2) 
            shape.setFillColor(defColorHits2);
        else 
            shape.setFillColor(defColorHits3);
    }

    void draw(sf::RenderWindow& mTarget) override 
    { 
        mTarget.draw(shape); 
    }
};