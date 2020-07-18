#include "Game.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "DrwBehaviour.h"

namespace drw
{
	Game::Game()
	{
		window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1280, 720), "Ping ping p0ng");
		window->setFramerateLimit(60);


		InitializeAssets();
		InitializeGameObjects();
	}

	void Game::Play()
	{
		currentGameState = GameState::ACTIVE;
		SetStartPositionForBall();
		while (window->isOpen())
		{
			ExitGame();

			Update();


			window->clear();

			DrawAssets();
			DrawGameObjects();

			window->display();
			
		}
		currentGameState = GameState::EXIT;
	}

	void Game::Update()
	{
		leftPaddle->Update(TIME_DELTA);
		rightPaddle->Update(TIME_DELTA);
	}

	void Game::InitializeScores()
	{
		playerOneScore = 0;
		playerTwoScore = 0;
	}

	void Game::InitializeAssets()
	{
		//// SOUND
		hitBuffer = std::make_unique<sf::SoundBuffer>();
		if (!hitBuffer->loadFromFile("sounds/hit.wav"))
		{
			std::cout << "ERR: hitbuffer missing!\n";
		}

		goalBuffer = std::make_unique<sf::SoundBuffer>();
		if (!goalBuffer->loadFromFile("sounds/goal.wav"))
		{
			std::cout << "ERR: goalbuffer is missing!\n";
		}

		//pingSound->setBuffer(hitBuffer);
		pingSound = std::make_unique<sf::Sound>(*hitBuffer);
		pongSound = std::make_unique<sf::Sound>(*hitBuffer);
		pongSound->setPitch(0.95f);

		borderSound = std::make_unique<sf::Sound>(*hitBuffer);
		borderSound->setPitch(1.05f);

		//// FONT
		ingameFont = std::make_unique<sf::Font>();
		if (!ingameFont->loadFromFile("text/ArcadeAlternate.ttf"))
		{
			std::cout << "ERR: ingameFont is missing!\n";
		}

		const float SCREEN_SIZE_X = static_cast<float>(window->getSize().x),
			SCREEN_SIZE_Y = static_cast<float>(window->getSize().y);

		playerOneScoreText = std::make_unique<sf::Text>("0", *ingameFont);
		playerOneScoreText->setCharacterSize(100);
		playerOneScoreText->setPosition((SCREEN_SIZE_X / 2) - (SCREEN_SIZE_X / 4) - (playerOneScoreText->getCharacterSize() / 2), SCREEN_SIZE_Y / 50);

		playerTwoScoreText = std::make_unique<sf::Text>("0", *ingameFont);
		playerTwoScoreText->setCharacterSize(100);
		playerTwoScoreText->setPosition((SCREEN_SIZE_X / 2) + (SCREEN_SIZE_X / 4) - (playerTwoScoreText->getCharacterSize() / 2), SCREEN_SIZE_Y / 50);
	}

	void Game::InitializeGameObjects()
	{
		const float SCREEN_SIZE_X = static_cast<float>(window->getSize().x), 
			SCREEN_SIZE_Y = static_cast<float>(window->getSize().y);

		//// BALL
		ball = std::make_unique<sf::CircleShape>(15.f);
		ball->setFillColor(sf::Color::Green);

		//// PADDLES
		leftPaddle = std::make_unique<drw::Box>();
		leftPaddle->SetPlayerControls(sf::Keyboard::W, sf::Keyboard::S);
		leftPaddle->SetBoxSize(sf::Vector2f(25.f, 75.f));
		leftPaddle->SetMoveSpeed(300.f);
		leftPaddle->SetPosition(
			40,
			(SCREEN_SIZE_Y / 2) - (leftPaddle->GetBoxHeight() / 2)
		);
		leftPaddle->SetColor(sf::Color::Blue);

		rightPaddle = std::make_unique<drw::Box>();
		rightPaddle->SetPlayerControls(sf::Keyboard::Up, sf::Keyboard::Down);
		rightPaddle->SetBoxSize(sf::Vector2f(25.f, 75.f));
		rightPaddle->SetMoveSpeed(300.f);
		rightPaddle->SetPosition(
			SCREEN_SIZE_X - 40 - rightPaddle->GetBoxWidth(),
			(SCREEN_SIZE_Y / 2) - (rightPaddle->GetBoxHeight() / 2)
		);
		rightPaddle->SetColor(sf::Color::Red);


		//// TOP/BOTTOM BORDERS
		topBorder = std::make_unique<sf::RectangleShape>(sf::Vector2<float>(SCREEN_SIZE_X, 5));
		topBorder->setFillColor(sf::Color::Cyan);
		topBorder->setPosition(0, -5);

		bottomBorder = std::make_unique<sf::RectangleShape>(sf::Vector2<float>(SCREEN_SIZE_X, 5));
		bottomBorder->setFillColor(sf::Color::Cyan);
		bottomBorder->setPosition(0, SCREEN_SIZE_Y + 5);

		//// GOAL POSTS
		playerOneGoalpost = std::make_unique<sf::RectangleShape>(sf::Vector2f(10, SCREEN_SIZE_Y));
		playerOneGoalpost->setFillColor(sf::Color::Green);
		playerOneGoalpost->setPosition(-2.f * ball->getRadius(), 0);

		playerTwoGoalpost = std::make_unique<sf::RectangleShape>(sf::Vector2f(10, SCREEN_SIZE_Y));
		playerTwoGoalpost->setFillColor(sf::Color::Green);
		playerTwoGoalpost->setPosition(SCREEN_SIZE_X + 2.f * ball->getRadius(), 0);


		//// MID BORDERS (visual)
		const int NUM_OF_BORDERS = 10;
		for (int i = 1; i < NUM_OF_BORDERS; i++)
		{
			std::unique_ptr<sf::RectangleShape> tempMidBorder = std::make_unique<sf::RectangleShape>(sf::Vector2f(10, 10));
			tempMidBorder->setFillColor(sf::Color::White);
			tempMidBorder->setPosition((SCREEN_SIZE_X / 2) - tempMidBorder->getSize().x / 2, i * (SCREEN_SIZE_Y / NUM_OF_BORDERS));
			midBordersVector.push_back(std::move(tempMidBorder));
		}
	}

	void Game::DrawGameObjects()
	{
		window->draw(*ball);

		//// DRAW PADDLES
		window->draw(leftPaddle->GetDraw());
		window->draw(rightPaddle->GetDraw());

		//// DRAW TOP/MID BORDERS ( too check to see if they're on screen )
		window->draw(*topBorder);
		window->draw(*bottomBorder);

		//// DRAW GOAL POSTS( too check to see if they're on screen )
		window->draw(*playerOneGoalpost);
		window->draw(*playerTwoGoalpost);

		//// DRAW MID BORDERS (visual)
		for (const auto& border : midBordersVector)
		{
			window->draw(*border);
		}

		
	}

	void Game::DrawAssets()
	{
		window->draw(*playerOneScoreText);
		window->draw(*playerTwoScoreText);
	}

	void Game::SetStartPositionForBall()
	{
		ball->setPosition(sf::Vector2f(
			(window->getSize().x / 2) - ball->getRadius(),
			(window->getSize().y / 2) - ball->getRadius()
		));
	}

	void Game::ExitGame()
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window->close();
		}
	}
}