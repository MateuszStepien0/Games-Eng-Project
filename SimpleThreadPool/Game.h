#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Grid.h"

class Game
{
public:
	Game();
	void run();
	sf::Time m_time{ sf::seconds(0) };

protected:
	void update(double dt);
	void render();
	void processEvents();

private:
	void initialise();
	sf::RenderWindow m_window;

	Grid grid;
};

