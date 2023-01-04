#include "Precompiled.h"
#include "SteeringModule.h"

using namespace SAVAGE::AI;

SteeringModule::SteeringModule(Agent& agent)
	: mAgent(agent)
{}

SAVAGE::Math::Vector2 SteeringModule::Calculate()
{
	SAVAGE::Math::Vector2 totalForce;
	for (auto& behavior : mBehaviors)
	{
		if (behavior->IsActive())
		{
			totalForce += behavior->Calculate(mAgent) * behavior->GetWeight();
		}
	}

	return totalForce;
}