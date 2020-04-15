#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "Box.h"
#include <memory>
#include <vector>
#include <string>
#include <random>

//class sf::RenderWindow;
//class sf::SoundBuffer;
//class sf::Sound;
//class sf::RectangleShape;
//class sf::CircleShape;
namespace drw
{
	enum GameState
	{
		ACTIVE, RETRY, PLAYER_ONE_SCORES, PLAYER_ONE_WINS, 
		PLAYER_TWO_SCORES, PLAYER_TWO_WINS, RESTART, PAUSE, EXIT
	};

	class Game
	{
	public:
		Game();
		~Game() {}
		void play();
	private:
		// FUNCS
		void initializeScores();
		void initializeAssets();
		void initializeGameObjects();
		void drawGameObjects();
		void drawAssets();

		void setStartPositionForBall();

		void exitGame();

		// VARS
		size_t playerOneScore, playerTwoScore;

		std::unique_ptr<sf::RenderWindow> window;
		std::unique_ptr<sf::SoundBuffer> hitBuffer, goalBuffer;
		std::unique_ptr<sf::Sound> pingSound, pongSound, borderSound, goalSound;
		std::unique_ptr<sf::Font> ingameFont;
		std::unique_ptr<sf::Text> playerOneScoreText, playerTwoScoreText;

		std::unique_ptr<sf::CircleShape> ball;
		std::unique_ptr<sf::RectangleShape> leftPaddle, rightPaddle, 
			topBorder, bottomBorder,
			playerOneGoalpost, playerTwoGoalpost;

		std::vector<std::unique_ptr<sf::RectangleShape>> midBordersVector;

		GameState currentGameState;
	};

}