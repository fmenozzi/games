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
	}

    void restart()
    {
        state = State::Start;

        current_world = std::array<bool, ROWS*COLS>{{false}};
        updated_world = std::array<bool, ROWS*COLS>{{false}};

        window.clear(sf::Color::White);
    }

    void processEvents()
    {
    	sf::Event event;
    	while (window.pollEvent(event)) {
    		if (event.type == sf::Event::Closed)
				window.close();
    	}
    }

    void update()
    {

    }

    void render()
    {
    	window.clear(sf::Color::White);
    	window.display();
    }

    void run()
    {
    	while (window.isOpen()) {
    		processEvents();
    		update();
    		render();
    	}
    }
};