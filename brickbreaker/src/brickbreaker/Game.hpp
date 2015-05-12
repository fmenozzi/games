#pragma once

#include <SFML/Graphics.hpp>

#include <brickbreaker/Manager.hpp>
#include <brickbreaker/CollisionManager.hpp>
#include <brickbreaker/Brick.hpp>
#include <brickbreaker/Paddle.hpp>

extern unsigned int WIN_WIDTH, WIN_HEIGHT;

class Game
{
private:
    enum class State{Paused, GameOver, InProgress, Victory};
    
    static constexpr int brkCountX{11};
    static constexpr int brkCountY{4};  

    static constexpr int brkStartColumn{1};
    static constexpr int brkStartRow{2};    

    static constexpr float brkSpacing{3.f};
    static constexpr float brkOffsetX{22.f};   

    sf::RenderWindow window{{WIN_WIDTH, WIN_HEIGHT}, "Brickbreaker"};
    
    Manager manager;

    sf::Font liberationSans;
    sf::Text textState, textLives;

    State state{State::GameOver};
    bool pausePressedLastFrame{false};
    
    int remainingLives{0};

public:
    Game() 
    { 
        window.setFramerateLimit(60); 

        liberationSans.loadFromFile(R"(/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf)");

        textState.setFont(liberationSans);
        textState.setCharacterSize(35.f);
        textState.setColor(sf::Color::Red);
        textState.setString("Paused");

        auto textBox = textState.getLocalBounds();
        textState.setOrigin(textBox.left + textBox.width/2.f, textBox.top + textBox.height/2.f);
        textState.setPosition(sf::Vector2f(WIN_WIDTH/2.f, WIN_HEIGHT/2.f));

        textLives.setFont(liberationSans);
        textLives.setPosition(14, 10);
        textLives.setCharacterSize(15.f);
        textLives.setColor(sf::Color::Red);
    }

    void restart()
    {
        remainingLives = 10;

        state = State::Paused;
        manager.clear();

        for (int iX{0}; iX < brkCountX; ++iX) {
            for (int iY{0}; iY < brkCountY; ++iY) {
                float x{(iX + brkStartColumn) * (Brick::defWidth + brkSpacing)};
                float y{(iY + brkStartRow) * (Brick::defHeight + brkSpacing)};

                auto& brick = manager.create<Brick>(brkOffsetX + x, y);

                brick.requiredHits = 1 + ((iX * iY) % 3);
            }
        }

        manager.create<Ball>(WIN_WIDTH / 2.f, WIN_HEIGHT / 2.f);
        manager.create<Paddle>(WIN_WIDTH / 2, WIN_HEIGHT - 50);
    }

    void run()
    {
        auto recenterTextBox = [&](const std::string& text) {
            auto textBox = textState.getLocalBounds();
            textState.setOrigin(textBox.left + textBox.width/2.f, textBox.top + textBox.height/2.f);
            textState.setPosition(sf::Vector2f(WIN_WIDTH/2.f, WIN_HEIGHT/2.f));
            textState.setString(text);
        };

        window.clear(sf::Color::White);

        while (true) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) 
                break;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
                if (!pausePressedLastFrame) {
                    if (state == State::Paused) 
                        state = State::InProgress;
                    else if (state == State::InProgress) 
                        state = State::Paused;
                }
                pausePressedLastFrame = true;
            } else {
                pausePressedLastFrame = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) 
                restart();

            // If the game is not in progress, do not draw or update            
            // game elements and display information to the player.
            if (state != State::InProgress) {
                if (state == State::Paused) 
                    recenterTextBox("Paused");
                else if (state == State::GameOver) 
                    recenterTextBox("Game Over!");
                else if (state == State::Victory) 
                    recenterTextBox("You Win!");

                window.draw(textState);         
            } else {
                // If there are no more balls on the screen, spawn a 
                // new one and remove a life.
                if (manager.getAll<Ball>().empty()) {
                    manager.create<Ball>(WIN_WIDTH / 2.f, WIN_HEIGHT / 2.f);
                    
                    --remainingLives;
                }

                // If there are no more bricks on the screen, 
                // the player won!
                if (manager.getAll<Brick>().empty()) 
                    state = State::Victory;

                // If the player has no more remaining lives, 
                // it's game over!
                if (remainingLives <= 0) 
                    state = State::GameOver;

                manager.update();

                manager.forEach<Ball>([this] (Ball& mBall) {
                    manager.forEach<Brick>([&mBall] (Brick& mBrick) {
                        CollisionManager::solveBrickBallCollision(mBrick, mBall);
                    });
                    manager.forEach<Paddle>([&mBall] (Paddle& mPaddle) {
                        CollisionManager::solvePaddleBallCollision(mPaddle, mBall);
                    });
                });

                manager.refresh();

                window.clear(sf::Color::White);
            
                manager.draw(window);

                textLives.setString("Lives: " + std::to_string(remainingLives));
                window.draw(textLives);
            }

            window.display();
        }   
    }
};