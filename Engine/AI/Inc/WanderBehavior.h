#pragma once
#include "SteeringBehavior.h"

namespace SAVAGE::AI
{
	class WanderBehavior : public SteeringBehavior
	{
	public:
		SAVAGE::Math::Vector2 Calculate(Agent& agent);
		void SetWanderRadius(const float radius) { wanderRadius = radius; }
		void SetWanderDistance(const float distance) { wanderDistance = distance; }
		void SetWanderJitter(const float jitter) { wanderJitter = jitter; }

		void SetParam(float radius, float distance, float jitter) { wanderRadius = radius; wanderDistance = distance; wanderJitter = jitter; }

		SAVAGE::Math::Vector2 wanderTargetLocal = SAVAGE::Math::Vector2::YAxis;
		
		float wanderRadius = 25.0f;
		float wanderDistance = 100.0f;
		float wanderJitter = 1.0f;
	};
}