#pragma once
#include <SFML/Graphics.hpp>

class Tile
{
public:
	Tile(sf::Vector2f rectSize, bool wall, int tid);
	void update();
	void render(sf::RenderWindow& window);

	void setPosition(sf::Vector2f pos);
	void setWall(bool wall);
	void setGoal(bool goal);
	void setCost(int tcost);
	void setMarked(bool tmarked);

	int getID();
	int getCost();
	bool getMarked();
	sf::Vector2f getPosition();
	std::vector<int> getNeighbours();
private:
	void initialise();

	sf::Vector2f position;
	sf::RectangleShape rect;
	std::vector<int> neighbourIDs;

	int id;
	bool isAWall = false;
	bool isAGoal = false;
	int cost;
	bool marked = false;
};

