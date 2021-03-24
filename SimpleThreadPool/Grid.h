#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "Tile.h"

class Grid
{
public:
	Grid();
	void update();
	void render(sf::RenderWindow& window);
	void processEvents(sf::Event& e);
private:
	void initialise();

	int selectedGrid;

	std::vector<Tile*> tiles30;
	std::vector<Tile*> tiles100;
	std::vector<Tile*> tiles1000;
};

