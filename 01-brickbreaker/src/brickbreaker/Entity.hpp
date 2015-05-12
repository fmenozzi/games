#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
    bool destroyed{false};

    virtual ~Entity() {}
    virtual void update() {} 
    virtual void draw(sf::RenderWindow& mTarget) {}
};