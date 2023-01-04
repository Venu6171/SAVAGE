#include "Precompiled.h"
#include "SeekBehavior.h"

#include "Agent.h"

using namespace SAVAGE;

//SAVAGE::Math::Vector2 AI::SeekBehavior::Calculate(Agent& agent)
//{
//	auto toTarget = agent.destination - agent.position;
//	auto desiredVelocity = SAVAGE::Math::Normalize(toTarget) * agent.maSAVAGE::Speed;
//	auto steeringForce = desiredVelocity - agent.velocity;
//
//	if (ShowDebug())
//	{
//		SAVAGE::DrawScreenLine(agent.position, agent.position + desiredVelocity, SAVAGE::::Colors::Red);
//		SAVAGE::DrawScreenLine(agent.position, agent.position + agent.velocity, SAVAGE::::Colors::Blue);
//	}
//
//	return steeringForce;
//}
