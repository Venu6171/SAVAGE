#pragma once
#include "SteeringBehavior.h"

namespace SAVAGE::AI
{
	class FleeBehavior : public SteeringBehavior
	{
	public:
		SAVAGE::Math::Vector2 Calculate(Agent& agent);
	};
}