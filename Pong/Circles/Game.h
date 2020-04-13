#pragma once
#include <iostream>

#include <SFML/Audio.hpp>
#include <cmath>
#include "Box.h"
#include <memory>
#include <vector>
#include <string>
#include <random>

class sf::RenderWindow;
class sf::SoundBuffer;
class sf::Sound;

class Game
{
public:
	Game(std::unique_ptr<sf::RenderWindow>& gameWindow);
	~Game() {}

	void play();
private:

	void initializeSoundAssets();
	void initializeGameObjects();

	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<sf::SoundBuffer> hitBuffer, goalBuffer;
	std::unique_ptr<sf::Sound> pingSound, pongSound, borderSound, goalSound;
};

