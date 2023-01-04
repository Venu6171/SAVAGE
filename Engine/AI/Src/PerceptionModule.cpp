#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace SAVAGE::AI;

PerceptionModule::PerceptionModule(Agent& agent, ImportanceCalculator calculator)
	: mAgent(agent)
	, ComputeImportance(std::move(calculator))
{

}

void SAVAGE::AI::PerceptionModule::Update(float deltaTime)
{
	//Update Sensors and refresh/add memory records
	for (auto& sensor : mSensors)
	{
		sensor->Update(mAgent, mMemory, deltaTime);
	}

	//Remove any records that are too old
	//mMemory.erase(std::remove_if(mMemory.begin(), mMemory.end(), [span = mMemorySpan](const auto& record) {
	//	return record.lastRecordedTime + span < SAVAGE::::GetTime();
	//}), mMemory.end());

	//Calculate importance on remaining records
	for (auto& record : mMemory)
	{
		record.importance = ComputeImportance(mAgent, record);
	}

	//Sort records by importance
	mMemory.sort([](const auto& a, auto& b)
	{
		return a.importance > b.importance;
	});
}