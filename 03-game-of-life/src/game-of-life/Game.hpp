#pragma once

#include <SFML/Graphics.hpp>

#include <game-of-life/Globals.hpp>

#include <array> 
#include <iostream>

class Game
{
private:
    enum class State {
        Start,
        Paused, 
        InProgress
    };

    sf::RenderWindow mWindow{{WIN_WIDTH, WIN_HEIGHT}, "Conway's Game of Life"};

    std::array<bool, ROWS*COLS>               mCurrentWorld = {{false}};
    std::array<bool, ROWS*COLS>               mUpdatedWorld = {{false}};
    std::array<sf::RectangleShape, ROWS*COLS> mGrid;
    
    sf::Font mLiberationSans;
    sf::Text mTextState;

    sf::Vector2i mMousePosition;

    sf::Color mCellColor{0,0,0,80};

    State mState{State::Paused};

    bool mPausePressedLastFrame{false};

public:
    Game()
    {
        mWindow.setVerticalSyncEnabled(true);

        mLiberationSans.loadFromFile(R"(../resources/fonts/LiberationSans-Regular.ttf)");

        // Text for game state
        mTextState.setFont(mLiberationSans);
        mTextState.setCharacterSize(35.f);
        mTextState.setColor(sf::Color::Black);
        mTextState.setString("Paused");
        auto mStateBox = mTextState.getLocalBounds();
        mTextState.setOrigin(mStateBox.left + mStateBox.width/2.f, 
                            mStateBox.top  + mStateBox.height/2.f);
        mTextState.setPosition(WIN_WIDTH/2.f, WIN_HEIGHT/2.f);

        // Setup grid
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                sf::RectangleShape cell;
                cell.setOrigin(SQUARE_SIZE / 2.f, SQUARE_SIZE / 2.f);
                cell.setPosition((c*SQUARE_SIZE)+(SQUARE_SIZE/2.f), (r*SQUARE_SIZE)+(SQUARE_SIZE/2.f));
                cell.setSize({SQUARE_SIZE, SQUARE_SIZE});
                cell.setFillColor(sf::Color::White);
                mGrid[idx(r,c)] = cell;
            }
        }
    }

    // Reset the state of the grid
    void restart()
    {
        mState = State::Start;

        mCurrentWorld.fill(false);
        mUpdatedWorld.fill(false);

        for (auto& cell : mGrid)
            cell.setFillColor(sf::Color::White);

        mWindow.clear(sf::Color::White);
    }

    // Enter game loop
    void run()
    {
        sf::Time sleepTime = sf::seconds(0.1f);
        while (mWindow.isOpen()) {
            if (mState == State::Start) {
                processStartEvents();
                render();
            } else {
                processEvents();
                render();
                if (mState == State::InProgress) {
                    update();
                    sf::sleep(sleepTime);
                } 
            }
        }
    }

private:
    // Process events in start state
    void processStartEvents()
    {
        sf::Event event;
        while (mWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                mWindow.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                mMousePosition = sf::Mouse::getPosition(mWindow);
                int mx = mMousePosition.x;
                int my = mMousePosition.y;
                if (0 <= mx and mx < WIN_WIDTH and 0 <= my and my < WIN_HEIGHT) {
                    int mGridIdx = idx(my / SQUARE_SIZE, mx / SQUARE_SIZE);

                    mGrid[mGridIdx].setFillColor(mCellColor);
                    mCurrentWorld[mGridIdx] = true;
                }
            }
        }

        // Start game
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
            mState = State::InProgress;
    }

    // Process external events
    void processEvents()
    {
        sf::Event event;
        while (mWindow.pollEvent(event)) { 
            // Close window
            if (event.type == sf::Event::Closed) {
                mWindow.close();
                return;
            } else if (event.type == sf::Event::KeyPressed) {
                // Pause game
                if (event.key.code == sf::Keyboard::Escape) {
                    if (mState == State::Paused) 
                        mState = State::InProgress;
                    else if (mState == State::InProgress)
                        mState = State::Paused;
                    return;
                } 

                // Restart game
                if (event.key.code == sf::Keyboard::R) {
                    restart();
                    return;
                }
            }
        }
    }

    void recenterTextBox(const std::string& text)
    {
        mTextState.setString(text);
        auto textBox = mTextState.getLocalBounds();
        mTextState.setOrigin(textBox.left + textBox.width/2.f, 
                             textBox.top  + textBox.height/2.f);
        mTextState.setPosition(WIN_WIDTH/2.f, WIN_HEIGHT/2.f);
    }

    // Update the state of the grid
    void update()
    {
        // Calculate updated world
        for (int r = 0; r < ROWS; r++)
            for (int c = 0; c < COLS; c++)
                mUpdatedWorld[idx(r,c)] = liveOrDie(r, c, adjacentNeighbors(r,c));

        // Update current world
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                mCurrentWorld[idx(r,c)] = mUpdatedWorld[idx(r,c)];
                mGrid[idx(r,c)].setFillColor(mCurrentWorld[idx(r,c)] ? mCellColor : sf::Color::White);
            }
        }
    }

    // Render the state of the grid to the window
    void render()
    {
        mWindow.clear(sf::Color::White);
        for (auto& cell : mGrid)
            mWindow.draw(cell);
        mWindow.display();
    }

    // Translate grid index to linear index
    int idx(int r, int c)
    {
        return COLS*r + c;
    }

    // Determine whether a given cell should live or die
    int liveOrDie(int r, int c, int n)
    {
        return n == 3 or (n == 2 and mCurrentWorld[idx(r,c)]);
    }

    // Count number of neighbors around each cell, making
    // sure not to count the cell itself
    int adjacentNeighbors(int r, int c)
    {
        int n = 0;
        for (int r1 = r-1; r1 < r+2; r1++)
            for (int c1 = c-1; c1 < c+2; c1++)
                if (mCurrentWorld[idx((r1+ROWS) % ROWS, (c1+COLS) % COLS)])
                    n++;

        if (mCurrentWorld[idx(r,c)])
            n--;

        return n;
    }
};
