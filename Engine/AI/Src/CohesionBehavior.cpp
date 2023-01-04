#include "Precompiled.h"
#include "CohesionBehavior.h"

#include "Agent.h"
#include "..\Inc\CohesionBehavior.h"

using namespace SAVAGE;

SAVAGE::Math::Vector2 AI::CohesionBehavior::Calculate(Agent& agent)
{
	SAVAGE::Math::Vector2 steeringForce;
	SAVAGE::Math::Vector2 centreMass = SAVAGE::Math::Vector2::Zero;

	int neighborCount = 0;

	for (auto neighbor : agent.neighbors)
	{
		centreMass += neighbor->position;
		neighborCount++;
	}

	if (neighborCount > 1)
	{
		centreMass /= neighborCount;
		auto desiredVelocity = SAVAGE::Math::Normalize(centreMass - agent.position) * agent.mSpeed;
		steeringForce = desiredVelocity - agent.velocity;
	}

	return steeringForce;
}
