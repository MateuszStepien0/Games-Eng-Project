#include "Game.h"
#include "Game.h"

static double const MS_PER_UPDATE = 10.0;

Game::Game()
	: m_window(sf::VideoMode(990, 990, 32), "Lab 7", sf::Style::Default)
{
	m_window.setVerticalSyncEnabled(true);

	initialise();
}

void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	while (m_window.isOpen())
	{
		sf::Time dt = clock.restart();

		lag += dt.asMilliseconds();
		m_time += dt;
		processEvents();

		while (lag > MS_PER_UPDATE)
		{
			update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}
		update(MS_PER_UPDATE);

		render();
	}
}

void Game::update(double dt)
{
	grid.update();
}

void Game::render()
{
	m_window.clear(sf::Color::Black);
	grid.render(m_window);
	m_window.display();
}

void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_window.close();
		}
	}
	grid.processEvents(event);
}

void Game::initialise()
{

}
