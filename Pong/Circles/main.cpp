// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "Box.h"
#include <memory>
#include <vector>
#include <string>
#include <random>
#include "Game.h"

void ExitGame(sf::RenderWindow &window);

void MovePaddle(sf::RectangleShape &paddle, const sf::Keyboard::Key &UP_KEY, const sf::Keyboard::Key &DOWN_KEY, const float MOVE_SPEED);

void DrawGame(sf::RenderWindow &window, const sf::CircleShape &ball, const sf::RectangleShape &leftPaddle, const sf::RectangleShape &rightPaddle);

void bounceBall(sf::Vector2f &ballDirection, const sf::Vector2f &ballDeltaVector);

void TriggerCheck(const sf::CircleShape &ball, const sf::RectangleShape& box, sf::Vector2f &dir, sf::Sound* &hitSound, const float ballSpeed);

bool IsBallCollidingWithBox(const sf::CircleShape &ball, const sf::RectangleShape &box);

void StartPositionBall(sf::CircleShape &ball);

const unsigned int SCREEN_SIZE_X = 1280, SCREEN_SIZE_Y = 720;

// for screen size
struct size
{
	const size_t x = 1280;
	const size_t y = 720;
}screen_size;

int main()
{
	std::unique_ptr<drw::Game> game = std::make_unique<drw::Game>();
	game->Play();

	return 0;

	sf::RenderWindow window(sf::VideoMode(screen_size.x, screen_size.y), "Ping Ping Pong!");
	window.setFramerateLimit(60);


	sf::CircleShape ball(15.f);
	ball.setFillColor(sf::Color::Green);
	StartPositionBall(ball);

	sf::RectangleShape leftPaddle(sf::Vector2<float>(20.f, 75.f));
	leftPaddle.setFillColor(sf::Color::Blue);
	leftPaddle.setPosition(
		40, 
		(SCREEN_SIZE_Y / 2) - (leftPaddle.getSize().y / 2)
	);
	

	sf::RectangleShape rightPaddle(sf::Vector2<float>(20.f, 75.f));
	rightPaddle.setFillColor(sf::Color::Red);
	rightPaddle.setPosition(
		SCREEN_SIZE_X - 40 - rightPaddle.getSize().x, 
		(SCREEN_SIZE_Y / 2) - (rightPaddle.getSize().y / 2)
	);
	
	sf::RectangleShape topBorder(sf::Vector2<float>(SCREEN_SIZE_X, 5));
	topBorder.setFillColor(sf::Color::Cyan);
	topBorder.setPosition(0,-5);

	sf::RectangleShape bottomBorder(sf::Vector2<float>(SCREEN_SIZE_X, 5));
	bottomBorder.setFillColor(sf::Color::Cyan);
	bottomBorder.setPosition(0, SCREEN_SIZE_Y+5);

	sf::RectangleShape playerOneGoalpost(sf::Vector2f(10, SCREEN_SIZE_Y));
	playerOneGoalpost.setFillColor(sf::Color::Green);
	playerOneGoalpost.setPosition(- 2.f * ball.getRadius(), 0);

	sf::RectangleShape playerTwoGoalpost(sf::Vector2f(10, SCREEN_SIZE_Y));
	playerTwoGoalpost.setFillColor(sf::Color::Green);
	playerTwoGoalpost.setPosition(SCREEN_SIZE_X + 2.f * ball.getRadius(), 0);

	std::vector<sf::RectangleShape> midBorders;
	const int numOfBorders = 10;
	for (int i = 1; i < numOfBorders; i++)
	{
		sf::RectangleShape temp(sf::Vector2f(10, 10));
		temp.setFillColor(sf::Color::White);
		temp.setPosition((SCREEN_SIZE_X / 2) - temp.getSize().x / 2, i * (SCREEN_SIZE_Y / numOfBorders));
		midBorders.emplace_back(temp);
	}

	//sf::Music music;
	//if (!music.openFromFile("music/mainMenu_final.wav"))
	//{
	//	return EXIT_FAILURE;
	//}

	//music.setVolume(25.f);
	//music.setLoop(true);
	//music.play();

	sf::Font ingameFont;
	if (!ingameFont.loadFromFile("text/ArcadeAlternate.ttf"))
	{
		return EXIT_FAILURE;
	}

	sf::Text playerOneScoreText("0", ingameFont), playerTwoScoreText("0", ingameFont);
	playerOneScoreText.setCharacterSize(100);
	playerOneScoreText.setPosition((SCREEN_SIZE_X / 2) - (SCREEN_SIZE_X / 4) - (playerOneScoreText.getCharacterSize() / 2), SCREEN_SIZE_Y / 50);

	playerTwoScoreText.setCharacterSize(100);
	playerTwoScoreText.setPosition((SCREEN_SIZE_X / 2) + (SCREEN_SIZE_X / 4) - (playerOneScoreText.getCharacterSize() / 2), SCREEN_SIZE_Y / 50);

	const float TIME_DELTA = 1/60.f;
	const float BALL_SPEED = 500.f;
	const float PADDLE_SPEED = 300.f;

	size_t playerOneScore = 0, playerTwoScore = 0;

	//sf::Vector2<float> ballDirection(-TIME_DELTA * BALL_SPEED, 0.f);
	sf::Vector2f ballDirection(-TIME_DELTA * BALL_SPEED, -TIME_DELTA * BALL_SPEED);
	sf::Vector2f ballVelocity(0.f, 0.f);

	//auto paddle = std::unique_ptr<drw::Box>(new drw::Box(
	//	25.f, 
	//	100.f, 
	//	sf::Color::White, 
	//	sf::Vector2f(20,(SCREEN_SIZE_Y / 2) - (leftPaddle.getSize().y / 2)), 
	//	true
	//));

	sf::SoundBuffer pingBuffer;
	if (!pingBuffer.loadFromFile("sounds/hit.wav"))
	{
		return EXIT_FAILURE;
	}

	sf::SoundBuffer goalBuffer;
	if (!goalBuffer.loadFromFile("sounds/goal.wav"))
	{
		return EXIT_FAILURE;
	}

	sf::Sound* pingSound = new sf::Sound(pingBuffer), 
		*pongSound = new sf::Sound(pingBuffer), 
		*borderSound = new sf::Sound(pingBuffer),
		*goalSound = new sf::Sound(goalBuffer);
	//auto pingSound = std::make_unique<sf::Sound>(new sf::Sound(pingBuffer));
	borderSound->setPitch(1.1f);
	pongSound->setPitch(0.9);

	std::random_device randomDevice;
	std::mt19937 mt(randomDevice());
	auto randomBool = std::uniform_int_distribution<int>(0, 1);

	float incrementedSpeedPerHit = 0.f;

	// game loop
	while (window.isOpen())
	{
		ExitGame(window);

		// player1 - up/down
		MovePaddle(leftPaddle, sf::Keyboard::W, sf::Keyboard::S, TIME_DELTA * PADDLE_SPEED);

		// player2 - up/down
		MovePaddle(rightPaddle, sf::Keyboard::Up, sf::Keyboard::Down, TIME_DELTA * PADDLE_SPEED);

		TriggerCheck(ball, leftPaddle, ballDirection, pingSound, BALL_SPEED);
		TriggerCheck(ball, rightPaddle, ballDirection, pongSound, BALL_SPEED);

		// static top border
		if (IsBallCollidingWithBox(ball, topBorder))
		{
			borderSound->play();
			bounceBall(ballDirection, ballVelocity);
		}

		// static bottom border
		if (IsBallCollidingWithBox(ball, bottomBorder))
		{
			borderSound->play();
			bounceBall(ballDirection, ballVelocity);
		}
		
		// player two wins
		if (IsBallCollidingWithBox(ball, playerOneGoalpost))
		{
			playerTwoScore++;
			playerTwoScoreText.setString(std::to_string(playerTwoScore));
			StartPositionBall(ball);
			goalSound->play();

			ballDirection = randomBool(mt) ?
				sf::Vector2f(-TIME_DELTA * BALL_SPEED, -TIME_DELTA * BALL_SPEED) :
				sf::Vector2f(-TIME_DELTA * BALL_SPEED, TIME_DELTA * BALL_SPEED);
		}

		// player one wins
		if (IsBallCollidingWithBox(ball, playerTwoGoalpost))
		{
			playerOneScore++;
			playerOneScoreText.setString(std::to_string(playerOneScore));
			StartPositionBall(ball);
			goalSound->play();

			ballDirection = randomBool(mt) ?  
				sf::Vector2f(TIME_DELTA * BALL_SPEED, -TIME_DELTA * BALL_SPEED):
				sf::Vector2f(TIME_DELTA * BALL_SPEED, TIME_DELTA * BALL_SPEED);
		}

		auto prev = ball.getPosition();
		ball.move(ballDirection);
		ballVelocity = ball.getPosition() - prev;
		ballVelocity = ballVelocity / (sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y)); // normizlied

		window.clear();

		//window.draw(paddle->GetBoxDraw());
		window.draw(topBorder);
		window.draw(bottomBorder);
		window.draw(playerOneGoalpost);
		window.draw(playerTwoGoalpost);
		window.draw(playerOneScoreText);
		window.draw(playerTwoScoreText);

		for (const auto& border : midBorders)
		{
			window.draw(border);
		}

		// Draw Things
		DrawGame(window, ball, leftPaddle, rightPaddle);
		window.display();
	}

	delete pongSound;
	delete pingSound;
	delete borderSound;
	delete goalSound;

	return 0;
}


void StartPositionBall(sf::CircleShape &ball)
{
	ball.setPosition(sf::Vector2f(
		(SCREEN_SIZE_X / 2) - ball.getRadius(),
		(SCREEN_SIZE_Y / 2) - ball.getRadius()
		));
}

bool IsBallCollidingWithBox(const sf::CircleShape &ball, const sf::RectangleShape &box)
{
	return (ball.getPosition().x < box.getPosition().x + box.getSize().x && // right side 
		ball.getPosition().x + 2.f * ball.getRadius() > box.getPosition().x) && // left side
		(ball.getPosition().y < box.getPosition().y + box.getSize().y && // top side
			ball.getPosition().y + 2.f * ball.getRadius() > box.getPosition().y); // bottom side
}

void TriggerCheck(const sf::CircleShape &ball, const sf::RectangleShape& box, sf::Vector2f &dir, sf::Sound* &hitSound, const float ballSpeed)
{
	if (IsBallCollidingWithBox(ball, box))
	{
		if(hitSound != nullptr && hitSound->getStatus() != hitSound->Playing)
		{
			hitSound->play();
		}

		int midpoint_x = box.getPosition().x + box.getSize().x / 2.f;
		int midpoint_y = box.getPosition().y + box.getSize().y / 2.f;
		sf::Vector2f boxCenter(midpoint_x, midpoint_y);
		sf::Vector2f ballCenter(ball.getPosition().x + ball.getRadius(), ball.getPosition().y + ball.getRadius());

		sf::Vector2f bounceVector = boxCenter - ballCenter;
		bounceVector = bounceVector / (sqrt(bounceVector.x * bounceVector.x + bounceVector.y * bounceVector.y)); // normilzie

		dir = -1.f*bounceVector * ballSpeed * (1/60.f);
	}
}

// bounce for borders box
void bounceBall(sf::Vector2f &ballDirection, const sf::Vector2f &ballVelocity)
{
	if (ballVelocity.x < 0 && ballVelocity.y > 0) // going left and up
	{
		ballDirection = sf::Vector2f(ballDirection.x, -1.f * ballDirection.y);
	}
	else if (ballVelocity.x > 0 && ballVelocity.y > 0) // going right and up
	{
		ballDirection = sf::Vector2f(ballDirection.x, -1.f * ballDirection.y);
	}
	else if (ballVelocity.x > 0 && ballVelocity.y < 0) // going right and down
	{
		ballDirection = sf::Vector2f(ballDirection.x, -1.f * ballDirection.y);
	}
	else if (ballVelocity.x < 0 && ballVelocity.y < 0) // going left and down
	{
		ballDirection = sf::Vector2f(ballDirection.x, -1.f * ballDirection.y);
	}
}

void DrawGame(sf::RenderWindow &window, const sf::CircleShape &ball, 
	const sf::RectangleShape &leftPaddle, const sf::RectangleShape &rightPaddle)
{
	window.draw(ball);
	window.draw(leftPaddle);
	window.draw(rightPaddle);
}

void MovePaddle(sf::RectangleShape &paddle, const sf::Keyboard::Key &UP_KEY, const sf::Keyboard::Key &DOWN_KEY, const float MOVE_SPEED)
{
	if (sf::Keyboard::isKeyPressed(UP_KEY))
	{
		if (paddle.getPosition().y > paddle.getSize().y/1.5f)
		{
			paddle.move(0.f, -1.f * MOVE_SPEED);
		}
	}
	else if (sf::Keyboard::isKeyPressed(DOWN_KEY))
	{
		if (paddle.getPosition().y < SCREEN_SIZE_Y - paddle.getSize().y - (paddle.getSize().y/1.5f))
		{
			paddle.move(0.f, MOVE_SPEED);
		}
	}
}

void ExitGame(sf::RenderWindow &window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		window.close();
	}
}
