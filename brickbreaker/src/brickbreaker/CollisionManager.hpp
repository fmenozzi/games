#include <brickbreaker/Paddle.hpp>
#include <brickbreaker/Ball.hpp>
#include <brickbreaker/Brick.hpp>

struct CollisionManager
{
    template<typename T1, typename T2> 
    static bool isIntersecting(const T1& mA, const T2& mB) noexcept
    {
        return mA.right()  >= mB.left() 
            && mA.left()   <= mB.right()
            && mA.bottom() >= mB.top() 
            && mA.top()    <= mB.bottom();
    }

    static void solvePaddleBallCollision(const Paddle& mPaddle, Ball& mBall) noexcept
    {
        if (!isIntersecting(mPaddle, mBall)) 
            return;

        mBall.velocity.y = -Ball::defVelocity;
        mBall.velocity.x = mBall.x() < mPaddle.x() ? -Ball::defVelocity : Ball::defVelocity;
    }

    static void solveBrickBallCollision(Brick& mBrick, Ball& mBall) noexcept
    {
        if (!isIntersecting(mBrick, mBall)) 
            return;
        
        // Instead of immediately destroying the brick upon collision,
        // we decrease and check its required hits first.
        --mBrick.requiredHits;
        if (mBrick.requiredHits <= 0) 
            mBrick.destroyed = true;

        float overlapLeft{mBall.right() - mBrick.left()};
        float overlapRight{mBrick.right() - mBall.left()};
        float overlapTop{mBall.bottom() - mBrick.top()};
        float overlapBottom{mBrick.bottom() - mBall.top()};

        bool ballFromLeft(std::abs(overlapLeft) < std::abs(overlapRight));
        bool ballFromTop(std::abs(overlapTop) < std::abs(overlapBottom));

        float minOverlapX{ballFromLeft ? overlapLeft : overlapRight};
        float minOverlapY{ballFromTop ? overlapTop : overlapBottom};

        if (std::abs(minOverlapX) < std::abs(minOverlapY))   
            mBall.velocity.x = ballFromLeft ? -Ball::defVelocity : Ball::defVelocity;
        else                                        
            mBall.velocity.y = ballFromTop ? -Ball::defVelocity : Ball::defVelocity; 
    } 
};