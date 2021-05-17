#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <queue>
#include <iostream>
#include <thread>

#include "Tile.h"
#include "NodeData.h"
#include "Graph.h"

typedef GraphArc<NodeData, int> Arc;
typedef GraphNode<NodeData, int> Node;

class Grid
{
public:
	// Constructor
	Grid();
	// Update function that updates functions
	void update();
	// Renders everything on the screen
	void render(sf::RenderWindow& window);
	// Processes events like key inputs
	void processEvents(sf::Event& e);

	// Returns the tile ID at the passed in position
	int getTileID(sf::Vector2f tilePos);

	// Graph objects for the 30x30 and 100x100 grids, separately
	Graph<NodeData, int> graph30{ 900 };
	Graph<NodeData, int> graph100{ 10000 };

private:
	// Self Explanatory
	void initialise();
	// Functions to create the grids
	void create30Grid();
	void create100Grid();
	//void create1000Grid();
	// Calculates costs of each tile to the goal
	void calculateCosts();
	// Calculates arcs between all nodes
	void setUpArcs();

	// Currently selected grid, updates the grid and tile size as well
	int selectedGrid;
	int gridSize;
	int tileSize;

	// Vector holding all tiles
	std::vector<Tile*> tiles;
	std::thread* aStarThreads[5];
	int goalTile;
};

#include "Enemy.h"