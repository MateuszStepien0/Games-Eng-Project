#include "Grid.h"

std::vector<Enemy*> enemies;

Grid::Grid()
{
	initialise();
}

void Grid::update()
{
	for (int i = 0; i < tiles.size(); i++)
	{
		tiles.at(i)->update();
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.at(i)->update();
	}
}

void Grid::render(sf::RenderWindow& window)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		tiles.at(i)->render(window);
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.at(i)->render(window);
	}
}

void Grid::processEvents(sf::Event& e)
{
	if (e.type == sf::Event::KeyReleased)
	{
		if (e.key.code == sf::Keyboard::Num1)
		{
			tiles.clear();
			enemies.clear();
			selectedGrid = 0;
			create30Grid();

			// Sets up the path for all enemies when the grid is created
			for (int i = 0; i < enemies.size(); i++)
			{
				graph30.aStar(graph30.m_nodes.at(getTileID(enemies.at(i)->getPosition())), graph30.m_nodes.at(0), enemies.at(i)->path);
			}
		}
		if (e.key.code == sf::Keyboard::Num2)
		{
			tiles.clear();
			enemies.clear();
			selectedGrid = 1;
			create100Grid();

			// Sets up the path for all enemies when the grid is created
			for (int i = 0; i < enemies.size(); i++)
			{
				graph100.aStar(graph100.m_nodes.at(getTileID(enemies.at(i)->getPosition())), graph100.m_nodes.at(0), enemies.at(i)->path);
			}
		}
		if (e.key.code == sf::Keyboard::Num3)
		{
			tiles.clear();
			selectedGrid = 2;
			//create1000Grid();
		}
	}
}

int Grid::getTileID(sf::Vector2f tilePos)
{
	if (selectedGrid == 0)
	{
		int tileX = tilePos.x;
		int tileY = tilePos.y;
		int modX = tileX % 30;
		int modY = tileY % 30;

		tileX = (tileX - modX) / 33;
		tileY = (tileY - modY) / 33;

		return ((tileY * 30) + tileX);
	}
	else if (selectedGrid == 1)
	{
		int tileX = tilePos.x;
		int tileY = tilePos.y;

		tileX /= 10;
		tileY /= 10;

		return ((tileY*100) + tileX);
	}
}

void Grid::initialise()
{
	create30Grid();

	for (int i = 0; i < enemies.size(); i++)
	{
		/*std::thread thread([&](Graph<NodeData, int>* graph) { graph->aStar(graph30.m_nodes.at(getTileID(enemies.at(i)->getPosition())), graph30.m_nodes.at(0), enemies.at(i)->path); }, &graph30);
		thread.detach();
		aStarThreads[i] = &thread;*/
		graph30.aStar(graph30.m_nodes.at(getTileID(enemies.at(i)->getPosition())), graph30.m_nodes.at(0), enemies.at(i)->path);
	}
}

void Grid::create30Grid()
{
	gridSize = 30;
	tileSize = 33;

	for (int i = 0; i < 30 * 30; i++)
	{
		int x, y;
		x = i % 30;
		y = i / 30;

		// Sets up all of the nodes when the grid is created, along with all data
		NodeData node;
		node.m_pos = sf::Vector2f(33 * x, 33 * y);
		node.m_id = i;
		node.m_isWalkable = true;
		node.m_pathCost = 0;
		node.m_heuristicCost = 0;

		// Created all of the tiles
		Tile* tile = new Tile(sf::Vector2f(32, 32), false, i);
		tile->setPosition(sf::Vector2f(33 * x, 33 * y));
		if (i == 0)
		{
			tile->setGoal(true);
			goalTile = i;
		}
		tiles.push_back(tile);
		graph30.addNode(node, (30 * y) + x);
	}

	int enemyID = 0;

	// Setting up all of the walls
	for (Tile* tile : tiles)
	{
		if (tile->getID() == 6)
		{
			for (int i = 0; i < 25; i++)
			{
				tiles.at(6 + i * 30)->setWall(true);
				graph30.m_nodes.at(6 + i * 30)->m_data.m_isWalkable = false;
			}
		}

		if (tile->getID() == 15)
		{
			for (int i = 5; i < 25; i++)
			{
				tiles.at(15 + i * 30)->setWall(true);
				graph30.m_nodes.at(15 + i * 30)->m_data.m_isWalkable = false;
			}
		}

		if (tile->getID() == 25)
		{
			for (int i = 28; i > 5; i--)
			{
				tiles.at(25 + i * 30)->setWall(true);
				graph30.m_nodes.at(25 + i * 30)->m_data.m_isWalkable = false;
			}
		}
	}

	// Setting up the enemies and randomizing their position
	for (int i = 0; i < 5; i++)
	{
		int randX = (rand() % 3) + 27;
		int randY = (rand() % 18) + 6;

		Enemy* enemy = new Enemy(enemyID, sf::Vector2f(32, 32));
		enemy->setPosition(tiles.at(randX + (randY * 30))->getPosition());
		enemies.push_back(enemy);
		enemyID++;
	}

	setUpArcs();
	calculateCosts();
}

void Grid::create100Grid()
{
	gridSize = 100;
	tileSize = 10;

	for (int i = 0; i < 100 * 100; i++)
	{
		int x, y;
		x = i % 100;
		y = i / 100;

		// Sets up all of the nodes when the grid is created, along with all data
		NodeData node;
		node.m_pos = sf::Vector2f(10 * x, 10 * y);
		node.m_id = i;
		node.m_isWalkable = true;
		node.m_pathCost = 0;
		node.m_heuristicCost = 0;

		// Creating all of the tiles
		Tile* tile = new Tile(sf::Vector2f(9, 9), false, i);
		tile->setPosition(sf::Vector2f(10 * x, 10 * y));
		if (i == 0)
		{
			tile->setGoal(true);
			goalTile = i;
		}
		tiles.push_back(tile);
		graph100.addNode(node, (100 * y) + x);
	}

	int enemyID = 0;

	// Setting up all of the walls
	for (Tile* tile : tiles)
	{
		if (tile->getID() == 15)
		{
			for (int i = 0; i < 80; i++)
			{
				tiles.at(15 + i * 100)->setWall(true);
				graph100.m_nodes.at(15 + i * 100)->m_data.m_isWalkable = false;
			}
		}

		if (tile->getID() == 30)
		{
			for (int i = 85; i > 5; i--)
			{
				tiles.at(30 + i * 100)->setWall(true);
				graph100.m_nodes.at(30 + i * 100)->m_data.m_isWalkable = false;
			}
		}

		if (tile->getID() == 42)
		{
			for (int i = 5; i < 90; i++)
			{
				tiles.at(42 + i * 100)->setWall(true);
				graph100.m_nodes.at(42 + i * 100)->m_data.m_isWalkable = false;
			}
		}

		if (tile->getID() == 60)
		{
			for (int i = 99; i > 10; i--)
			{
				tiles.at(60 + i * 100)->setWall(true);
				graph100.m_nodes.at(60 + i * 100)->m_data.m_isWalkable = false;
			}
		}

		if (tile->getID() == 72)
		{
			for (int i = 8; i < 85; i++)
			{
				tiles.at(72 + i * 100)->setWall(true);
				graph100.m_nodes.at(72 + i * 100)->m_data.m_isWalkable = false;
			}
		}

		if (tile->getID() == 84)
		{
			for (int i = 5; i < 95; i++)
			{
				tiles.at(84 + i * 100)->setWall(true);
				graph100.m_nodes.at(84 + i * 100)->m_data.m_isWalkable = false;
			}
		}
	}

	// Setting up the enemies and randomizing their position
	for (int i = 0; i < 50; i++)
	{
		int randX = (rand() % 12) + 87;
		int randY = (rand() % 80) + 10;

		Enemy* enemy = new Enemy(enemyID, sf::Vector2f(9,9));
		enemy->setPosition(tiles.at(randX + (randY * 100))->getPosition());
		enemies.push_back(enemy);
		enemyID++;
	}

	setUpArcs();
	calculateCosts();
}

/*void Grid::create1000Grid()
{
	for (int i = 0; i < 1000 * 1000; i++)
	{
		int x, y;
		x = i % 1000;
		y = i / 1000;

		Tile* tile = new Tile(sf::Vector2f(1, 1), false);
		tile->setPosition(sf::Vector2f(x, y));
		tiles.push_back(tile);
	}
}*/

void Grid::calculateCosts()
{
	for (auto i : tiles)
	{
		i->setCost(0);
		i->setMarked(false);
	}

	std::queue<int> tileQueue;
	tileQueue.push(tiles.at(goalTile)->getID());
	tiles.at(goalTile)->setCost(0);
	tiles.at(goalTile)->setMarked(true);

	while (tileQueue.size() != 0)
	{
		std::vector<int> n = tiles.at(tileQueue.front())->getNeighbours();

		for (int i = 0; i < n.size(); i++)
		{
			if (tiles.at(n.at(i))->getMarked() == false)
			{
				tiles.at(n.at(i))->setMarked(true);
				tiles.at(n.at(i))->setCost(tiles.at(tileQueue.front())->getCost() + 1);
				tileQueue.push(tiles.at(n.at(i))->getID());
			}
		}
		tileQueue.pop();
	}
}

void Grid::setUpArcs()
{
	for (int row = 0; row < gridSize; row++)
	{
		for (int col = 0; col < gridSize; col++)
		{
			for (int direction = 0; direction < 9; direction++)
			{
				if (direction == 4)
				{
					continue;
				}

				int n_row = row + ((direction % 3) - 1);
				int n_col = col + ((direction / 3) - 1);

				if (n_row >= 0 && n_row < gridSize && n_col >= 0 && n_col < gridSize)
				{
					if (n_row == row || n_col == col)
					{
						if (selectedGrid == 0)
						{
							graph30.addArc((col * 30 + row), (n_col * 30 + n_row), tileSize);
						}
						else if (selectedGrid == 1)
						{
							graph100.addArc((col * 100 + row), (n_col * 100 + n_row), tileSize);
						}
					}
					else
					{
						if (selectedGrid == 0)
						{
							graph30.addArc((col * 30 + row), (n_col * 30 + n_row), tileSize * 2);
						}
						else if (selectedGrid == 1)
						{
							graph100.addArc((col * 100 + row), (n_col * 100 + n_row), tileSize * 2);
						}
					}
				}
			}
		}
	}
}
