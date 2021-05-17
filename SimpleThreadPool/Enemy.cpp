#include "Enemy.h"

Enemy::Enemy(int tid, sf::Vector2f size)
{
	initialise();
	rect.setSize(size);
}

void Enemy::update()
{
	if (!finished)
	{
		if (!path.empty() && index != 0)
		{
			if (counter == timer)
			{
				setPosition(path.at(index - 1)->m_data.m_pos);
				
				if (index > 0)
				{
					index--;
					if (index == 1)
					{
						finished = true;
					}
				}

				counter = 0;
			}
			else
			{
				counter++;
			}
		}
		else
		{
			index = path.size();
		}
	}
}

void Enemy::render(sf::RenderWindow& window)
{
	window.draw(rect);
}

sf::Vector2f Enemy::getPosition()
{
	return position;
}

void Enemy::setPosition(sf::Vector2f pos)
{
	position = pos;
	rect.setPosition(position);
}

void Enemy::initialise()
{
	rect.setPosition(sf::Vector2f(0, 0));
	rect.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));

	counter = 0;
	index = path.size();
}
