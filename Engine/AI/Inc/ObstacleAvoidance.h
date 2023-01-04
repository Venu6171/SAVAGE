#pragma once
#include "SteeringBehavior.h"
#include "AIWorld.h"

namespace SAVAGE::AI
{
	class ObstacleAVoidance : public SteeringBehavior
	{
	public:
		//using Obstacles = std::vector<SAVAGE::Math::Circle>;

		SAVAGE::Math::Vector2 Calculate(Agent& agent);
	};
}