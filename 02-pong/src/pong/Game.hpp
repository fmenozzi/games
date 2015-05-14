#pragma once

#include <SFML/Graphics.hpp>

#include <pong/Manager.hpp>
#include <pong/CollisionManager.hpp>
#include <pong/Paddle.hpp>

extern unsigned int WIN_WIDTH, WIN_HEIGHT;

class Game
{
private:
    enum class State {
        Paused, 
        InProgress, 
        LeftVictory,
        RightVictory
    };

    sf::RenderWindow window{{WIN_WIDTH, WIN_HEIGHT}, "Pong"};
    
    Manager manager;

    sf::Font liberationSans;
    sf::Text textState, textLeftScore, textRightScore;

    State state{State::Paused};

    bool pausePressedLastFrame{false};

    int leftScore{0}, rightScore{0};
    static constexpr int MAX_SCORE{10};

public:
    Game() 
    { 
        window.setFramerateLimit(60); 

        liberationSans.loadFromFile(R"(../resources/fonts/LiberationSans-Regular.ttf)");

        // Text for game state
        textState.setFont(liberationSans);
        textState.setCharacterSize(35.f);
        textState.setColor(sf::Color::Black);
        textState.setString("Paused");
        auto stateBox = textState.getLocalBounds();
        textState.setOrigin(stateBox.left + stateBox.width/2.f, 
                            stateBox.top  + stateBox.height/2.f);
        textState.setPosition(WIN_WIDTH/2.f, WIN_HEIGHT/2.f);

        // Text for left player's score
        textLeftScore.setString("0");
        textLeftScore.setFont(liberationSans);
        textLeftScore.setColor(sf::Color::Black);
        textLeftScore.setCharacterSize(30.f);
        auto leftScoreBox = textLeftScore.getLocalBounds();
        textLeftScore.setOrigin(leftScoreBox.left + leftScoreBox.width/2.f, 
                                leftScoreBox.top  + leftScoreBox.height/2.f);
        textLeftScore.setPosition(WIN_WIDTH/2.f - (10 + leftScoreBox.width/2.f), 30);

        // Text for right player's score
        textRightScore.setString("0");
        textRightScore.setFont(liberationSans);
        textRightScore.setColor(sf::Color::Black);
        textRightScore.setCharacterSize(30.f);
        auto rightScoreBox = textRightScore.getLocalBounds();
        textRightScore.setOrigin(rightScoreBox.left + rightScoreBox.width/2.f, 
                                 rightScoreBox.top  + rightScoreBox.height/2.f);
        textRightScore.setPosition(WIN_WIDTH/2.f + (10 + rightScoreBox.width/2.f), 30);
    }

    void restart()
    {
        leftScore  = 0;
        rightScore = 0;

        state = State::Paused;

        manager.clear();

        manager.create<Ball>(WIN_WIDTH / 2.f, WIN_HEIGHT / 2.f);

        manager.create<Paddle>(20.f,             WIN_HEIGHT / 2.f);
        manager.create<Paddle>(WIN_WIDTH - 20.f, WIN_HEIGHT / 2.f);

        window.clear(sf::Color::White);
    }

    bool handleKeyboardInput()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) 
            return false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
            if (not pausePressedLastFrame) {
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

        return true;
    }

    void run()
    {
        auto recenterTextBox = [this](const std::string& text) {
            textState.setString(text);

            auto textBox = textState.getLocalBounds();
            textState.setOrigin(textBox.left + textBox.width/2.f, textBox.top + textBox.height/2.f);
            textState.setPosition(sf::Vector2f(WIN_WIDTH/2.f, WIN_HEIGHT/2.f));
            
        };

        window.clear(sf::Color::White);

        while (true) {
            if (not handleKeyboardInput())
                break;

            if (state == State::InProgress) {
                if (manager.getAll<Ball>().empty()) {
                    manager.create<Ball>(WIN_WIDTH / 2.f, WIN_HEIGHT / 2.f);

                    if (manager.getAll<Ball>()[0]->onLeftSideOfScreen)
                        leftScore++;
                    else
                        rightScore++;
                }

                if (leftScore == MAX_SCORE)
                    state = State::LeftVictory;
                else if (rightScore == MAX_SCORE)
                    state = State::RightVictory;

                manager.update();

                manager.forEach<Ball>([this] (Ball& mBall) {
                    manager.forEach<Paddle>([&mBall] (Paddle& mPaddle) {
                        CollisionManager::solvePaddleBallCollision(mPaddle, mBall);
                    });
                });

                manager.refresh();

                window.clear(sf::Color::White);
            
                manager.draw(window);

                textLeftScore.setString(std::to_string(leftScore));
                textRightScore.setString(std::to_string(rightScore));
                window.draw(textLeftScore);
                window.draw(textRightScore);
            } else {
                if (state == State::Paused) 
                    recenterTextBox("Paused");
                else if (state == State::LeftVictory) 
                    recenterTextBox("Left Wins!");
                else if (state == State::RightVictory)
                    recenterTextBox("Right Wins!");

                window.draw(textState);  
            }

            window.display();
        }   
    }
};