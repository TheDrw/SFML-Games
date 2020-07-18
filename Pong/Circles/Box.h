#pragma once

#include <SFML/Graphics.hpp>
// TODO - figure out how to fwd declare w/in namespace
//class sf::CircleShape;
//class sf::RectangleShape;
//class sf::Color;
//class sf::Keyboard;
namespace drw
{


	class Box
	{
	public:
		Box();
		Box(const sf::Vector2f &boxSize, const sf::Color &color, const sf::Vector2f &startPosition, const bool enablePhysics);
		~Box() {}

		void Update(float deltaTime);
		
		void PerformCollision(const sf::CircleShape &ball);
		void Move(const sf::Vector2f &direction);

		// GITTIRS
		bool IsOverlappingWithBall(const sf::CircleShape &ball);
		bool IsCollisionEnabled() const;
		bool IsReceivingInput() const;
		
		sf::Vector2f GetPosition() const;
		sf::RectangleShape const GetDraw();
		sf::Vector2f GetBoxSize() const;
		float GetBoxWidth() const;
		float GetBoxHeight() const;
		float GetMoveSpeed() const;

		// SITTIRZ
		void SetStatic(const bool isStatic);
		void SetCollisionEnabled(const bool enableCollision);
		void SetMoveSpeed(const float moveSpeed);
		void SetColor(const sf::Color &boxColor);
		void SetBoxSize(const sf::Vector2f &boxSize);
		void SetPosition(const sf::Vector2f &position);
		void SetPosition(const float x, const float y);
		void SetPlayerControls(const sf::Keyboard::Key &upInput, const sf::Keyboard::Key &downInput);

	private:
		void InstantiateBox();

		// VARS
		sf::Keyboard::Key _upInput, _downInput;
		sf::Vector2f _boxSize;
		float _moveSpeed;
		sf::Color _color;
		sf::Vector2f _position, _velocity;
		bool _isCollisionEnabled, _isStatic;
		sf::RectangleShape _box;
	};
}

