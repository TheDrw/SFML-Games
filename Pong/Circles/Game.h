#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "Box.h"
#include <memory>
#include <vector>
#include <string>
#include <random>

#include "DrwBehaviour.h"
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

	class Game// : DrwBehaviour
	{
	public:
		Game();
		~Game() {}
		void Play();
		
	private:
		// FUNCS
		void Update();
		void InitializeScores();
		void InitializeAssets();
		void InitializeGameObjects();
		void DrawGameObjects();
		void DrawAssets();
		//bool isCurrentObjectCollidingWithOpposingObject();

		void SetStartPositionForBall();

		void ExitGame();

		// VARS
		size_t playerOneScore, playerTwoScore;

		std::unique_ptr<drw::Box> leftPaddle, rightPaddle;

		std::unique_ptr<sf::RenderWindow> window;
		std::unique_ptr<sf::SoundBuffer> hitBuffer, goalBuffer;
		std::unique_ptr<sf::Sound> pingSound, pongSound, borderSound, goalSound;
		std::unique_ptr<sf::Font> ingameFont;
		std::unique_ptr<sf::Text> playerOneScoreText, playerTwoScoreText;

		std::unique_ptr<sf::CircleShape> ball;
		std::unique_ptr<sf::RectangleShape> topBorder, bottomBorder,
			playerOneGoalpost, playerTwoGoalpost;

		std::vector<std::unique_ptr<sf::RectangleShape>> midBordersVector;

		GameState currentGameState;

		// CONST
		float TIME_DELTA = 1 / 60.f;
	};

}