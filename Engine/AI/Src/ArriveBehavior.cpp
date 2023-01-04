#include "Precompiled.h"
#include "ArriveBehavior.h"

#include "Agent.h"
#include "..\Inc\ArriveBehavior.h"

using namespace SAVAGE;

//SAVAGE::Math::Vector2 AI::ArriveBehavior::Calculate(Agent& agent)
//{
//	auto desiredVelocity = agent.destination - agent.position;
//	auto distance = SAVAGE::Math::Magnitude(desiredVelocity);
//
//	slowingDistance = 500.0f;
//
//	if (distance < slowingDistance)
//	{
//		desiredVelocity = SAVAGE::Math::Normalize(desiredVelocity) * agent.maSAVAGE::Speed * (distance / slowingDistance);
//	}
//	else
//	{
//		desiredVelocity = SAVAGE::Math::Normalize(desiredVelocity) * agent.maSAVAGE::Speed;
//	}
//
//	auto steeringForce = desiredVelocity - agent.velocity;
//
//	return steeringForce;
//}
