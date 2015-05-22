#pragma once

#include <SFML/Graphics.hpp>

#include <game-of-life/Globals.hpp>

#include <array> 

class Game
{
private:
    enum class State {
        Start,
        Paused, 
        InProgress
    };

    sf::RenderWindow window{{WIN_WIDTH, WIN_HEIGHT}, "Conway's Game of Life"};

    std::array<bool, ROWS*COLS> current_world = {{false}};
    std::array<bool, ROWS*COLS> updated_world = {{false}};
    
    sf::Font liberationSans;
    sf::Text textState;

    State state{State::Paused};

    bool pausePressedLastFrame{false};

public:
	Game()
	{

	}

    void restart()
    {
        state = State::Start;

        window.clear(sf::Color::White);
    }

    void run()
    {

    }
};