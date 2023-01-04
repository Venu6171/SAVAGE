#include "PreCompiled.h"
#include "AlignmentBehavior.h"

#include "Agent.h"

using namespace SAVAGE;

SAVAGE::Math::Vector2 AI::AlignmentBehavior::Calculate(Agent& agent)
{
	SAVAGE::Math::Vector2 alignment;
	int neighborCount = 0;

	for (auto neighbor : agent.neighbors)
	{
		if (neighbor != &agent)
		{
			alignment += neighbor->heading;
			neighborCount++;
		}
	}

	if (neighborCount > 0)
	{
		alignment /= neighborCount;
		alignment -= agent.heading;
	}

	return alignment;
}