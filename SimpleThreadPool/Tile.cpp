#include "Tile.h"

Tile::Tile(sf::Vector2f rectSize, bool wall, int tid)
{
	isAWall = wall;
	id = tid;

	initialise();
	rect.setSize(rectSize);
}

void Tile::update()
{
	if (isAWall)
	{
		rect.setFillColor(sf::Color(0, 0, 0));
		cost = 999;
	}
	else if (isAGoal)
	{
		rect.setFillColor(sf::Color(0, 255, 0));
	}
	else
	{
		rect.setFillColor(sf::Color(60, 60, 60));
	}

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

void Tile::setWall(bool wall)
{
	isAWall = wall;
}

void Tile::setGoal(bool goal)
{
	isAGoal = goal;
}

void Tile::setCost(int tcost)
{
	cost = tcost;
}

void Tile::setMarked(bool tmarked)
{
	marked = tmarked;
}

int Tile::getID()
{
	return id;
}

int Tile::getCost()
{
	return cost;
}

bool Tile::getMarked()
{
	return marked;
}

sf::Vector2f Tile::getPosition()
{
	return position;
}

std::vector<int> Tile::getNeighbours()
{
	return neighbourIDs;
}

void Tile::initialise()
{
	rect.setSize(sf::Vector2f(32, 32));
	if (isAWall)
	{
		rect.setFillColor(sf::Color(0, 0, 0));
	}
	else
	{
		rect.setFillColor(sf::Color(60, 60, 60));
	}
}
