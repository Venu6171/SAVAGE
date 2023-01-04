#include "Precompiled.h"
#include "SeparationBehavior.h"

#include "Agent.h"
#include "..\Inc\SeparationBehavior.h"

using namespace SAVAGE;

//SAVAGE::Math::Vector2 AI::SeparationBehavior::Calculate(Agent& agent)
//{
//	SAVAGE::Math::Vector2 separation;
//
//	for (auto neighbor : agent.neighbors)
//	{
//		//Don't Compute force  for self
//		auto neighborToAgent = agent.position - neighbor->position;
//		auto lenghtSqr = SAVAGE::Math::MagnitudeSqr(neighborToAgent);
//
//		if (lenghtSqr <= 0.0f)
//			continue;
//
//		separation += neighborToAgent / lenghtSqr;
//	}
//	
//	if (ShowDebug())
//	{
//		
//	}
//
//	return separation;
//}
