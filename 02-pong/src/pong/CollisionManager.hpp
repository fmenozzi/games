#pragma once 

#include <pong/Paddle.hpp>
#include <pong/Ball.hpp>

struct CollisionManager
{
    static bool isIntersecting(const Paddle& paddle, const Ball& ball) noexcept
    {
        return paddle.right()  >= ball.left() 
           and paddle.left()   <= ball.right()
           and paddle.bottom() >= ball.top() 
           and paddle.top()    <= ball.bottom();
    }

    static void solvePaddleBallCollision(const Paddle& paddle, Ball& ball) noexcept
    {
        if (not isIntersecting(paddle, ball))
            return;

        float overlapLeft{ball.right() - paddle.left()};
        float overlapRight{paddle.right() - ball.left()};
        float overlapTop{ball.bottom() - paddle.top()};
        float overlapBottom{paddle.bottom() - ball.top()};

        bool ballFromLeft{std::abs(overlapLeft) < std::abs(overlapRight)};
        bool ballFromTop{std::abs(overlapTop) < std::abs(overlapBottom)};

        float minOverlapX{ballFromLeft ? overlapLeft : overlapRight};
        float minOverlapY{ballFromTop ? overlapTop : overlapBottom};

        if (std::abs(minOverlapX) < std::abs(minOverlapY))   
            ball.velocity.x = ballFromLeft ? -Ball::speed : Ball::speed;
        else                                        
            ball.velocity.y = ballFromTop ? -Ball::speed : Ball::speed; 
    }
};