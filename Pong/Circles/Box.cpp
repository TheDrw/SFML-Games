#include "Box.h"
#include <iostream>


namespace drw
{
	Box::Box() :
		_color(sf::Color::Magenta), _position(sf::Vector2f(0.f, 0.f)), _isCollisionEnabled(false)
	{
		_boxSize = sf::Vector2f(25.f, 25.f);
		InstantiateBox();
	}

	Box::Box(const sf::Vector2f &boxSize, const sf::Color &color, const sf::Vector2f &startPosition, const bool enableCollision)
		: _color(color), _position(startPosition), _isCollisionEnabled(enableCollision), _isStatic(false)
	{
		SetBoxSize(boxSize);
		InstantiateBox();
	}

	void Box::Update(const float deltaTime)
	{
		if (IsReceivingInput())
		{
			auto moveDirection = sf::Vector2f(0.f, 0.f);
			if (sf::Keyboard::isKeyPressed(_upInput))
			{
				moveDirection = sf::Vector2f(0.f, -1.f * _moveSpeed * deltaTime);
			}
			else if (sf::Keyboard::isKeyPressed(_downInput))
			{
				moveDirection = sf::Vector2f(0.f, _moveSpeed * deltaTime);
			}

			Move(moveDirection);
		}
	}

	bool Box::IsOverlappingWithBall(const sf::CircleShape & ball)
	{
		const float WIDTH = _boxSize.x, HEIGHT = _boxSize.y;

		if ((ball.getPosition().x < _position.x + WIDTH && // right part 
			ball.getPosition().x + 2.f * ball.getRadius() > _position.x) && // left part
			(ball.getPosition().y < _position.y + HEIGHT && // top
				ball.getPosition().y + 2.f * ball.getRadius() > _position.y) // bottom
			)
		{
			std::cout << "ball is overlapping\n";
			if (_isCollisionEnabled)
			{
				PerformCollision(ball);
			}

			
			return true;
		}

		return false;
	}

	bool Box::IsCollisionEnabled() const
	{
		return _isCollisionEnabled;
	}

	bool Box::IsReceivingInput() const
	{
		
		return sf::Keyboard::isKeyPressed(_upInput) ||
			sf::Keyboard::isKeyPressed(_downInput);
		
	}

	sf::Vector2f Box::GetPosition() const
	{
		return _position;
	}

	sf::RectangleShape const Box::GetDraw()
	{
		return _box;
	}

	sf::Vector2f Box::GetBoxSize() const
	{
		return _boxSize;
	}

	float Box::GetBoxWidth() const
	{
		return _boxSize.x;
	}

	float Box::GetBoxHeight() const
	{
		return _boxSize.y;
	}

	float Box::GetMoveSpeed() const
	{
		return _moveSpeed;
	}

	void Box::SetStatic(const bool isStatic)
	{
		_isStatic = isStatic;
	}

	void Box::SetCollisionEnabled(const bool enableCollision)
	{
		_isCollisionEnabled = enableCollision;
	}

	void Box::SetMoveSpeed(const float moveSpeed)
	{
		_moveSpeed = moveSpeed;
	}

	void Box::SetColor(const sf::Color & boxColor)
	{
		_box.setFillColor(boxColor);
	}

	void Box::SetBoxSize(const sf::Vector2f & boxSize)
	{
		_boxSize = boxSize;
		_box.setSize(boxSize);
	}

	void Box::SetPosition(const sf::Vector2f & position)
	{
		_position = position;
		_box.setPosition(position);
	}

	void Box::SetPosition(const float x, const float y)
	{
		_position = sf::Vector2f(x, y);
		_box.setPosition(_position);
	}

	void Box::SetPlayerControls(const sf::Keyboard::Key & upInput, const sf::Keyboard::Key & downInput)
	{
		_upInput = upInput;
		_downInput = downInput;
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
		_box = sf::RectangleShape(sf::Vector2f(_boxSize.x, _boxSize.y));
		_box.setFillColor(_color);
		_box.setPosition(_position);
	}
}