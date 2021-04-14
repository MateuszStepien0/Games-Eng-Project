#include "Grid.h"

Grid::Grid()
{
	initialise();
	selectedGrid = 1;
}

void Grid::update()
{
	switch (selectedGrid)
	{
	case 1:
		for (int i = 0; i < tiles30.size(); i++)
		{
			tiles30.at(i)->update();
		}
		break;
	case 2:
		for (int i = 0; i < tiles100.size(); i++)
		{
			tiles100.at(i)->update();
		}
		break;
	case 3:
		for (int i = 0; i < tiles1000.size(); i++)
		{
			tiles1000.at(i)->update();
		}
		break;
	}
}

void Grid::render(sf::RenderWindow& window)
{
	switch (selectedGrid)
	{
	case 1:
		for (int i = 0; i < tiles30.size(); i++)
		{
			tiles30.at(i)->render(window);
		}
		break;
	case 2:
		for (int i = 0; i < tiles100.size(); i++)
		{
			tiles100.at(i)->render(window);
		}
		break;
	case 3:
		for (int i = 0; i < tiles1000.size(); i++)
		{
			tiles1000.at(i)->render(window);
		}
		break;
	}
}

void Grid::processEvents(sf::Event& e)
{
	if (e.type == sf::Event::KeyReleased)
	{
		if (e.key.code == sf::Keyboard::Num1)
		{
			selectedGrid = 1;
		}
		if (e.key.code == sf::Keyboard::Num2)
		{
			selectedGrid = 2;
		}
		if (e.key.code == sf::Keyboard::Num3)
		{
			selectedGrid = 3;
		}
	}
}

void Grid::initialise()
{
	for (int i = 0; i < 30 * 30; i++)
	{
		int x, y;
		x = i % 30;
		y = i / 30;

		Tile* tile = new Tile(sf::Vector2f(32,32), false);
		tile->setPosition(sf::Vector2f(33 * x, 33 * y));
		tiles30.push_back(tile);
	}

	for (int i = 0; i < 100 * 100; i++)
	{
		int x, y;
		x = i % 100;
		y = i / 100;

		Tile* tile = new Tile(sf::Vector2f(9,9), false);
		tile->setPosition(sf::Vector2f(10 * x, 10 * y));
		tiles100.push_back(tile);
	}

	/*for (int i = 0; i < 1000 * 1000; i++)
	{
		int x, y;
		x = i % 1000;
		y = i / 1000;

		Tile* tile = new Tile(sf::Vector2f(0, 0), false);
		tile->setPosition(sf::Vector2f(x, y));
		tiles1000.push_back(tile);
	}*/
}
