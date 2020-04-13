#include "Game.h"
#include <SFML/Graphics.hpp>



Game::Game(std::unique_ptr<sf::RenderWindow>& gameWindow)
{
	window = std::move(gameWindow);
}

void Game::play()
{
	while (window->isOpen())
	{

	}
}

void Game::initializeSoundAssets()
{
}

void Game::initializeGameObjects()
{
}
