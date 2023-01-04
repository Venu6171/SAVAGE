#pragma once
#include "SteeringBehavior.h"

enum class Deceleration
{
	slow = 3,
	normal = 2,
	fast = 1
};

namespace SAVAGE::AI
{
	class ArriveBehavior : public SteeringBehavior
	{
	public:
		SAVAGE::Math::Vector2 Calculate(Agent& agent);

		Deceleration decel = Deceleration::normal;

		float slowingDistance = 0.0f;
	};
}