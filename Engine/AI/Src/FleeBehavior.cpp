#include "Precompiled.h"
#include "FleeBehavior.h"

#include "Agent.h"
#include "..\Inc\FleeBehavior.h"

SAVAGE::::Math::Vector2 AI::FleeBehavior::Calculate(Agent& agent)
{
	auto toTarget = agent.position - agent.destination;
	auto desiredVelocity = SAVAGE::::Math::Normalize(toTarget) * agent.maSAVAGE::Speed;

	auto steeringForce = desiredVelocity - agent.velocity;

	return steeringForce;
}
