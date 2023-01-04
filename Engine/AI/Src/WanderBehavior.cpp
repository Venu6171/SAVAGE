#include "Precompiled.h"
#include "WanderBehavior.h"

#include "Agent.h"
#include "..\Inc\WanderBehavior.h"

using namespace SAVAGE;

//SAVAGE::Math::Vector2 AI::WanderBehavior::Calculate(Agent& agent)
//{
//	auto newTarget = wanderTargetLocal + SAVAGE::RandomUnitCircle() * wanderJitter;
//	wanderTargetLocal = SAVAGE::Math::Normalize(newTarget) * wanderRadius;
//	auto projectedTarget = wanderTargetLocal + SAVAGE::Math::Vector2{ 0.0f, wanderDistance };
//	auto worldTarget = SAVAGE::Math::TransformCoord(projectedTarget, agent.LocalToWorld());
//
//	auto toDestination = worldTarget - agent.position;
//	auto desiredVelocity = SAVAGE::Math::Normalize(toDestination) * agent.maSAVAGE::Speed;
//	auto steeringForce = desiredVelocity - agent.velocity;
//
//	auto wanderCentreCircle = SAVAGE::Math::TransformCoord(SAVAGE::Math::Vector2{ 0.0f, wanderDistance }, agent.LocalToWorld());
//
//	SAVAGE::DrawScreenCircle(wanderCentreCircle, wanderRadius, SAVAGE::::Colors::Red);
//	SAVAGE::DrawScreenCircle(worldTarget, 5.0f, SAVAGE::Graphics::Colors::Yellow);
//
//	return steeringForce;
//}
