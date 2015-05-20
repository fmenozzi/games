#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
    bool destroyed{false};
    bool onLeftSideOfScreen{false};
    bool onUpperSideOfScreen{false};

    virtual ~Entity() {}
    virtual void update(sf::Time dt) {} 
    virtual void draw(sf::RenderWindow& mTarget) {}

    virtual float x() const noexcept = 0;
    virtual float y() const noexcept = 0;
};