#pragma once
#include <string>
#include <SFML/Graphics.hpp>

// ---------------------------------------------------------------------
//  Name:           NodeData
//  Description:    A simple struct to encapsulate all the node data.
//                  Add or remove fields as required.               
// ---------------------------------------------------------------------
struct NodeData
{
	int m_id;
	sf::Vector2f m_pos;
	bool m_isWalkable;
	int m_pathCost;
	int m_heuristicCost;
};
