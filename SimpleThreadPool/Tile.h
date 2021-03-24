#pragma once
#include <SFML/Graphics.hpp>
class Tile
{
public:
	Tile(sf::Vector2f rectSize);
	void update();
	void render(sf::RenderWindow& window);
	void setPosition(sf::Vector2f pos);
private:
	void initialise();

	sf::Vector2f position;
	sf::RectangleShape rect;
};

