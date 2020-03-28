// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "main.h"

void MovePaddle(sf::RectangleShape &paddle, const sf::Keyboard::Key &UP_KEY, const sf::Keyboard::Key &DOWN_KEY, const float MOVE_SPEED);

void DrawGame(sf::RenderWindow &window, const sf::CircleShape &ball, const sf::RectangleShape &leftPaddle, const sf::RectangleShape &rightPaddle);

const unsigned int SCREEN_SIZE_X = 800, SCREEN_SIZE_Y = 600;

int main()
{

	sf::RenderWindow window(sf::VideoMode(SCREEN_SIZE_X, SCREEN_SIZE_Y), "SFML CIRCLES!");
	window.setFramerateLimit(60);

	sf::CircleShape ball(25.f);
	ball.setFillColor(sf::Color::Green);
	ball.setPosition(sf::Vector2<float>(
		(SCREEN_SIZE_X /2) - ball.getRadius(), 
		(SCREEN_SIZE_Y/2) - ball.getRadius()
		));

	sf::RectangleShape leftPaddle(sf::Vector2<float>(25.f, 100.f));
	leftPaddle.setFillColor(sf::Color::Blue);
	leftPaddle.setPosition(
		20, 
		(SCREEN_SIZE_Y / 2) - (leftPaddle.getSize().y / 2)
	);
	
	sf::RectangleShape rightPaddle(sf::Vector2<float>(25.f, 100.f));
	rightPaddle.setFillColor(sf::Color::Red);
	rightPaddle.setPosition(
		SCREEN_SIZE_X - 20 - rightPaddle.getSize().x, 
		(SCREEN_SIZE_Y / 2) - (rightPaddle.getSize().y / 2)
	);
	
	//sf::Music music;
	//if (!music.openFromFile("music/mainMenu_final.wav")) return EXIT_FAILURE;
	//music.setLoop(true);
	//music.play();

	const float TIME_DELTA = 1/60.f;
	const float BALL_SPEED = 150.f;
	
	const float PADDLE_SPEED = 200.f;

	sf::Vector2<float> ballDirection(-TIME_DELTA * BALL_SPEED, 0.f);
	// game loop
	while (window.isOpen())
	{
		ExitGame(window);


		// player1 - up/down
		MovePaddle(leftPaddle, sf::Keyboard::W, sf::Keyboard::S, TIME_DELTA * PADDLE_SPEED);

		// player2 - up/down
		MovePaddle(rightPaddle, sf::Keyboard::Up, sf::Keyboard::Down, TIME_DELTA * PADDLE_SPEED);

		if ((ball.getPosition().x < leftPaddle.getPosition().x + leftPaddle.getSize().x && // right part 
			ball.getPosition().x + ball.getRadius() > leftPaddle.getSize().x ) && // left part
			(ball.getPosition().y < leftPaddle.getPosition().y + leftPaddle.getSize().y && // top
				ball.getPosition().y + 2.f * ball.getRadius() > leftPaddle.getPosition().y) // bottom
			)
		{
			ballDirection = -1.f * ballDirection;
		}

		if ((ball.getPosition().x + 2.f * ball.getRadius() > rightPaddle.getPosition().x &&  // left part of paddle 
			ball.getPosition().x < rightPaddle.getPosition().x + rightPaddle.getSize().x) && // right part of paddle
			(ball.getPosition().y < rightPaddle.getPosition().y + rightPaddle.getSize().y &&  // top
			ball.getPosition().y + 2.f * ball.getRadius() > rightPaddle.getPosition().y) // bottom
			)
		{
			ballDirection = -1.f * ballDirection;
		}


		ball.move(ballDirection);

		window.clear();

		// Draw Things
		DrawGame(window, ball, leftPaddle, rightPaddle);

		
		window.display();
	}

	return 0;
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
		if (paddle.getPosition().y > 0.f)
		{
			paddle.move(0.f, -1.f * MOVE_SPEED);
		}
	}
	else if (sf::Keyboard::isKeyPressed(DOWN_KEY))
	{
		if (paddle.getPosition().y < SCREEN_SIZE_Y - paddle.getSize().y)
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
