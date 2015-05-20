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
        Start,
        Paused, 
        InProgress, 
        LeftVictory,
        RightVictory
    };

    sf::RenderWindow window{{WIN_WIDTH, WIN_HEIGHT}, "Pong"};
    
    sf::Font liberationSans;
    sf::Text textState, textLeftScore, textRightScore;

    State state{State::Paused};

    bool pausePressedLastFrame{false};

    int leftScore{0}, rightScore{0};

    static constexpr int MAX_SCORE{10};

public:
    Game() 
    { 
        window.setVerticalSyncEnabled(true);

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
        textLeftScore.setPosition(WIN_WIDTH/2.f - (15 + leftScoreBox.width/2.f), 30);

        // Text for right player's score
        textRightScore.setString("0");
        textRightScore.setFont(liberationSans);
        textRightScore.setColor(sf::Color::Black);
        textRightScore.setCharacterSize(30.f);
        auto rightScoreBox = textRightScore.getLocalBounds();
        textRightScore.setOrigin(rightScoreBox.left + rightScoreBox.width/2.f, 
                                 rightScoreBox.top  + rightScoreBox.height/2.f);
        textRightScore.setPosition(WIN_WIDTH/2.f + (15 + rightScoreBox.width/2.f), 30);
    }

    void restart()
    {
        leftScore  = 0;
        rightScore = 0;

        state = State::Start;

        Manager::createBall(WIN_WIDTH/2.f, WIN_HEIGHT/2.f);

        Manager::createLeftPaddle(20.f,              WIN_HEIGHT/2.f);
        Manager::createRightPaddle(WIN_WIDTH - 20.f, WIN_HEIGHT/2.f);

        window.clear(sf::Color::White);
    }

    void handleKeyboardInput()
    {
        // Close window
        sf::Event event;
        while (window.pollEvent(event)) { 
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }

        // Pause game
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            if (not pausePressedLastFrame) {
                if (state == State::Paused or state == State::Start) 
                    state = State::InProgress;
                else if (state == State::InProgress)
                    state = State::Paused;
            }
            pausePressedLastFrame = true;
        } else {
            pausePressedLastFrame = false;
        }

        // Restart game
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) 
            restart();
    }

    void run()
    {
        auto recenterTextBox = [this](const std::string& text) {
            textState.setString(text);

            auto textBox = textState.getLocalBounds();
            textState.setOrigin(textBox.left + textBox.width/2.f, 
                                textBox.top  + textBox.height/2.f);
            textState.setPosition(WIN_WIDTH/2.f, WIN_HEIGHT/2.f);
            
        };

        window.clear(sf::Color::White);

        sf::Clock clock;
        sf::Time  timeSinceLastUpdate = sf::Time::Zero;
        sf::Time  TimePerFrame = sf::seconds(1.f / 60.f);
        while (window.isOpen()) {
            handleKeyboardInput();

            timeSinceLastUpdate += clock.restart();

            while (timeSinceLastUpdate > TimePerFrame) {
                timeSinceLastUpdate -= TimePerFrame;

                handleKeyboardInput();

                if (state == State::InProgress) {
                    if (Manager::ball.destroyed) {
                        if (Manager::ball.onLeftSideOfScreen)
                            rightScore++;
                        else
                            leftScore++;

                        Manager::createBall(WIN_WIDTH/2.f, WIN_HEIGHT/2.f);
                    }

                    if (leftScore == MAX_SCORE) {
                        state = State::LeftVictory;
                    } else if (rightScore == MAX_SCORE) {
                        state = State::RightVictory;
                    } else {
                        Manager::update(TimePerFrame);

                        CollisionManager::solvePaddleBallCollision(Manager::leftPaddle,  Manager::ball);
                        CollisionManager::solvePaddleBallCollision(Manager::rightPaddle, Manager::ball);

                        window.clear(sf::Color::White);
                    
                        Manager::draw(window);

                        textLeftScore.setString(std::to_string(leftScore));
                        textRightScore.setString(std::to_string(rightScore));
                        window.draw(textLeftScore);
                        window.draw(textRightScore);
                    }
                } else {
                    if (state == State::Paused) 
                        recenterTextBox("Paused");
                    else if (state == State::Start)
                        recenterTextBox("Press Esc to start");
                    else if (state == State::LeftVictory) 
                        recenterTextBox("Left Wins!");
                    else if (state == State::RightVictory)
                        recenterTextBox("Right Wins!");

                    window.draw(textState);  
                }
            }

            window.display();
        }   
    }
};