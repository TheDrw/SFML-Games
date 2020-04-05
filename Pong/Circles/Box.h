#pragma once

#include <SFML/Graphics.hpp>
namespace drw
{
	class Box
	{
	public:
		Box();
		Box(const float width, const float height, const sf::Color &color, const sf::Vector2f &startPosition, const bool enablePhysics);
		~Box() {}

		void Update();
		bool IsOverlapping(const sf::CircleShape &ball);
		void PerformCollision(const sf::CircleShape &ball);
		void Move(const sf::Vector2f &direction);
		sf::Vector2f GetPosition() const { return _position; }
		void SetSimulatingPhysics(const bool isSimulatingPhysics) { _isSimulatingPhysics = isSimulatingPhysics; }
		bool IsSimulatingPhysics() const { return _isSimulatingPhysics; }
		sf::RectangleShape GetBoxDraw() const { return _box; }
	private:

		void InstantiateBox();

		float _width, _height;
		sf::Color _color;
		sf::Vector2f _position, _velocity;
		bool _isSimulatingPhysics;
		sf::RectangleShape _box;
	};
}