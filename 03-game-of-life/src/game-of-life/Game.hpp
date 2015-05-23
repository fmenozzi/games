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

    sf::RenderWindow window{{WIN_WIDTH, WIN_HEIGHT}, "Conway's Game of Life"};

    std::array<bool, ROWS*COLS>               current_world = {{false}};
    std::array<bool, ROWS*COLS>               updated_world = {{false}};
    std::array<sf::RectangleShape, ROWS*COLS> grid          = {{}};
    
    sf::Font liberationSans;
    sf::Text textState;

    State state{State::Paused};

    bool pausePressedLastFrame{false};

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

        // Setup grid
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                sf::RectangleShape cell;
                cell.setOrigin(SQUARE_SIZE / 2.f, SQUARE_SIZE / 2.f);
                cell.setPosition((c*SQUARE_SIZE)+(SQUARE_SIZE/2.f), (r*SQUARE_SIZE)+(SQUARE_SIZE/2.f));
                cell.setSize({SQUARE_SIZE, SQUARE_SIZE});
                cell.setFillColor(sf::Color::White);
                grid[idx(r,c)] = cell;
            }
        }
    }

    // Reset the state of the grid
    void restart()
    {
        state = State::Start;

        current_world.fill(false);
        updated_world.fill(false);

        // Add glider in upper left corner
        current_world[idx(0,1)] = true;
        current_world[idx(1,2)] = true;
        current_world[idx(2,0)] = true;
        current_world[idx(2,1)] = true;
        current_world[idx(2,2)] = true;

        // Add some other random shit
        for (int i = 0; i < ROWS; i++)
            current_world[idx(i % 11, (i + 3) % 7)] = true;
        for (int i = 0; i < COLS; i++)
            current_world[idx(i % 7, (i + 5) % 11)] = true;

        window.clear(sf::Color::White);
    }

    // Enter game loop
    void run()
    {
        sf::Time sleepTime = sf::seconds(0.1f);
        while (window.isOpen()) {
            processEvents();
            render();
            update();
            sf::sleep(sleepTime);
        }
    }

private:
    // Process external events
    void processEvents()
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    // Update the state of the grid
    void update()
    {
        // Calculate updated world
        for (int r = 0; r < ROWS; r++)
            for (int c = 0; c < COLS; c++)
                updated_world[idx(r,c)] = liveOrDie(r, c, adjacentNeighbors(r,c));

        // Update current world
        for (int r = 0; r < ROWS; r++)
            for (int c = 0; c < COLS; c++) {
                current_world[idx(r,c)] = updated_world[idx(r,c)];
                grid[idx(r,c)].setFillColor(current_world[idx(r,c)] ? sf::Color::Black : sf::Color::White);
            }
    }

    // Render the state of the grid to the window
    void render()
    {
        window.clear(sf::Color::White);
        for (auto& cell : grid)
            window.draw(cell);
        window.display();
    }

    // Translate grid index to linear index
    int idx(int r, int c)
    {
        return COLS*r + c;
    }

    // Determine whether a given cell should live or die
    int liveOrDie(int r, int c, int n)
    {
        return n == 3 || (n == 2 && current_world[idx(r,c)]);
    }

    // Count number of neighbors around each cell, making
    // sure not to count the cell itself
    int adjacentNeighbors(int r, int c)
    {
        int n = 0;
        for (int r1 = r-1; r1 < r+2; r1++)
            for (int c1 = c-1; c1 < c+2; c1++)
                if (current_world[idx((r1+ROWS) % ROWS, (c1+COLS) % COLS)])
                    n++;

        if (current_world[idx(r,c)])
            n--;

        return n;
    }
};