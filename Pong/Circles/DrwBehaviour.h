#pragma once
namespace drw
{
	class DrwBehaviour
	{
	public:
		virtual void Update(const float deltaTime) = 0;
	};

}