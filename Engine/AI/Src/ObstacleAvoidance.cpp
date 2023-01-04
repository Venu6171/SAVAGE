#include "Precompiled.h"

#include "..\Inc\ObstacleAvoidance.h"
#include "AIWorld.h"

using namespace SAVAGE;

//SAVAGE::Math::Vector2 SAVAGE::AI::ObstacleAVoidance::Calculate(Agent& agent)
//{
//	auto& obstacles = agent.world.GetObstacles();
//
//	auto desiredVelocity = agent.destination - agent.position;
//	auto detectionLength = 100.0f + (SAVAGE::Math::Magnitude(desiredVelocity) / agent.maSAVAGE::Speed) * 100.0f;
//
//	Obstacles localObstacles;
//
//	for (auto& obs : obstacles)
//	{
//
//		if (SAVAGE::::Math::Distance(agent.position, obs.center) < detectionLength + obs.radius)
//		{
//			continue;
//		}
//
//		auto toWorld = agent.LocalToWorld();
//		auto toLocal = SAVAGE::::Math::Inverse(toWorld);
//		auto localCentre = SAVAGE::::Math::TransformCoord(obs.center, toLocal);
//		auto worldCentre = SAVAGE::::Math::TransformCoord(localCentre, toWorld);
//
//		if (localCentre.y >= 0)
//		{
//			localObstacles.push_back({ localCentre, obs.radius + agent.radius });
//		}
//	}
//
//	for (auto& o : localObstacles)
//	{
//		o.radius += agent.radius;
//	}
//
//	for (auto& o : localObstacles)
//	{
//		if (abs(o.center.SAVAGE::) > o.radius)
//		{
//			localObstacles.clear();
//		}
//	}
//
//	float closestY = 0.1f;
//	SAVAGE::::Math::Circle closestOb;
//
//	for (auto& o : localObstacles)
//	{
//		float SAVAGE:: = 0.0f;
//		float y1 = 0.0f;
//		float y2 = 0.0f;
//
//		y1 = abs(sqrt(pow((o.radius - o.center.SAVAGE::), 2)) + o.center.y);
//
//		if (y1 > 0 && y1 < closestY)
//		{
//			closestY = y1;
//			closestOb = o;
//		}
//
//		if (y2 > 0 && y2 < closestY)
//		{
//			closestY = y2;
//			closestOb = o;
//		}
//
//		SAVAGE::::Math::Vector2 lateral = {};
//		SAVAGE::::Math::Vector2 braking = {0, };
//	}
//
//	return detectionLength;
//}
