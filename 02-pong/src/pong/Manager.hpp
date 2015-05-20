#pragma once

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

    static void update(sf::Time dt)                           
    { 
        ball.update(dt);

        bool ballisMovingUp = ball.velocity.y < 0;
        bool isPlayerPaddle = true;
        leftPaddle.update(dt, isPlayerPaddle, ballisMovingUp);
        rightPaddle.update(dt, not isPlayerPaddle, ballisMovingUp);
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