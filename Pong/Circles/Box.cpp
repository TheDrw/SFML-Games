#include "Box.h"
#include <iostream>

namespace drw
{
	Box::Box() :
		_width(25.f), _height(25.f), _color(sf::Color::Magenta), _position(sf::Vector2f(0.f, 0.f)), _isSimulatingPhysics(false)
	{
		InstantiateBox();
	}

	Box::Box(const float width, const float height, const sf::Color &color, const sf::Vector2f &startPosition, const bool enablePhysics)
		: _width(width), _height(height), _color(color), _position(startPosition), _isSimulatingPhysics(enablePhysics)
	{
		InstantiateBox();
	}

	bool Box::IsOverlapping(const sf::CircleShape & ball)
	{
		if ((ball.getPosition().x < _position.x + _width && // right part 
			ball.getPosition().x + 2.f * ball.getRadius() > _position.x) && // left part
			(ball.getPosition().y < _position.y + _height && // top
				ball.getPosition().y + 2.f * ball.getRadius() > _position.y) // bottom
			)
		{
			if (_isSimulatingPhysics)
			{
				PerformCollision(ball);
			}

			std::cout << "ball is overlapping\n";
			return true;
		}

		return false;
	}

	void Box::PerformCollision(const sf::CircleShape & ball)
	{

	}

	void Box::Move(const sf::Vector2f & direction)
	{
		auto prev_pos = _box.getPosition();

		_box.move(direction);
		_position = _box.getPosition();

		_velocity = prev_pos - _position;
	}

	void Box::InstantiateBox()
	{
		_box = sf::RectangleShape(sf::Vector2f(_width, _height));
		_box.setFillColor(_color);
		_box.setPosition(_position);
	}
}