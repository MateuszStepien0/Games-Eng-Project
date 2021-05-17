#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Grid.h"
#include "Graph.h"

class Enemy
{
public:
	Enemy(int tid, sf::Vector2f size);
	void update();
	void render(sf::RenderWindow& window);
	sf::Vector2f getPosition();

	void setPosition(sf::Vector2f pos);

	std::vector<Node*> path;

private:
	void initialise();

	sf::Vector2f position;
	sf::RectangleShape rect;

	int id;
	int timer = 50;
	int counter = 0;
	int index;
	bool finished = false;
};