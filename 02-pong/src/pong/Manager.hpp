#pragma once

#include <vector>
#include <map>
#include <memory>
#include <typeinfo>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include <pong/make_unique.hpp>
#include <pong/Entity.hpp>
#include <pong/Ball.hpp>
#include <pong/Paddle.hpp>

class Manager
{
public:
    static Ball ball;
    static Paddle leftPaddle, rightPaddle;

    static Ball& createBall(float x, float y)
    {
        ball.init(x,y);
        ball.destroyed = false;
        return ball;
    }

    static Paddle& createLeftPaddle(float x, float y)
    {
        leftPaddle.init(x,y);
        return leftPaddle;
    }

    static Paddle& createRightPaddle(float x, float y)
    {
        rightPaddle.init(x,y);
        return rightPaddle;
    }

    static void update()                           
    { 
        ball.update();

        bool ballisMovingUp = ball.velocity.y < 0;
        leftPaddle.update(true, ballisMovingUp);
        rightPaddle.update(false, ballisMovingUp);
    }

    static void draw(sf::RenderWindow& target)    
    { 
        ball.draw(target);

        leftPaddle.draw(target);
        rightPaddle.draw(target);
    }
};

Ball Manager::ball{0,0};
Paddle Manager::leftPaddle{0,0}, Manager::rightPaddle{0,0};