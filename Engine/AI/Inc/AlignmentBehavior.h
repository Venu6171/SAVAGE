#pragma once
#include "SteeringBehavior.h"

namespace SAVAGE::AI
{
	class AlignmentBehavior : public SteeringBehavior
	{
	public:
		SAVAGE::Math::Vector2 Calculate(Agent& agent);
	};
}