#include "Tile.h"

Tile::Tile(sf::Vector2f rectSize)
{
	initialise();
	rect.setSize(rectSize);
}

void Tile::update()
{
}

void Tile::render(sf::RenderWindow& window)
{
	window.draw(rect);
}

void Tile::setPosition(sf::Vector2f pos)
{
	position = pos;
	rect.setPosition(position);
}

void Tile::initialise()
{
	rect.setSize(sf::Vector2f(32, 32));
	rect.setFillColor(sf::Color(30, 30, 30));
}
