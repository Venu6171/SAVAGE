#include "GameState.h"
#include <fstream>
#include <sstream>

using namespace SAVAGE;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;
using namespace SAVAGE::ML;

namespace
{
	int mPopulationSize = 100;
	int maxGeneValue = 3;
	int chromoLength = 40;
	float mMutationRate = 0.1f;
	float mCrossOverRate = 0.45f;

}

void GameState::Initialize()
{
	mStartPos = { 15.0f, 35.0f };
	mEndPos = { 785.0f, 580.0f };
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		SAVAGE::MainApp().Quit();

	if (mInitialized && !mStop)
	{
		auto& best = mGeneticAlgorithm.GetBestGenome();

		if (!mLast)
		{
			mGeneticAlgorithm.Advance();

			std::string bestStr;
			//Print the next Best Genome
			for (auto& gene : mGeneticAlgorithm.GetBestGenome().chromosome)
				bestStr += static_cast<char>((gene + 48));
			mAppLog.AddLog("Generation[%d] %s Fitness: %f\n", mGeneticAlgorithm.GetGeneration(), bestStr.c_str(), mGeneticAlgorithm.GetBestGenome().fitness);
		}
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Values", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::InputFloat("MutationRate", &mMutationRate, 0.1f, 1.0f, "%0.2f");
	ImGui::InputFloat("CrossOverRate", &mCrossOverRate, 0.1f, 1.0f, "%0.2f");
	ImGui::InputInt("ChromoLength", &chromoLength, 1, 100);


	if (ImGui::Button("Run"))
	{
		auto GeneratePopulation = []()
		{
			std::vector<GeneticAlgorithm::Genome> mPopulation;
			mPopulation.resize(mPopulationSize);
			for (auto& genome : mPopulation)
			{
				genome.chromosome.reserve(chromoLength);
				for (size_t i = 0; i < chromoLength; ++i)
					genome.chromosome.push_back(RandomInt(0, maxGeneValue));
			}

			return mPopulation;
		};
		auto ComputeFitness = [endPos = mEndPos, startPos = mStartPos](auto& genome)
		{
			ImVec2 nextPos = startPos;
			for (auto direction : genome.chromosome)
			{
				if (direction == 0 && nextPos.y >= 30.0f)
					nextPos.y -= 30.0f;

				if (direction == 1 && nextPos.y < 580.0f - 30.0f)
					nextPos.y += 30.0f;

				if (direction == 2 && nextPos.x >= 30.0f)
					nextPos.x -= 30.0f;

				if (direction == 3 && nextPos.y < 785.0f - 30.0f)
					nextPos.x += 30.0f;
			}
			float fitness = Distance({ endPos.x, endPos.y, 0.0f }, { nextPos.x, nextPos.y, 0.0f });
			return 1000 - fitness;
		};
		auto MateFunction = [](const auto& parent1, const auto& parent2)
		{
			GeneticAlgorithm::Genome offspring;
			offspring.chromosome.reserve(parent1.chromosome.size());

			for (size_t i = 0; i < parent1.chromosome.size(); ++i)
			{
				if (RandomFloat() < mMutationRate)
					offspring.chromosome.push_back(RandomInt(0, maxGeneValue));
				else if (RandomFloat() < mCrossOverRate)
					offspring.chromosome.push_back(parent1.chromosome[i]);
				else
					offspring.chromosome.push_back(parent2.chromosome[i]);
			}

			return offspring;
		};



		mAppLog.AddLog("Initializing new population....\n");

		mGeneticAlgorithm.Initialize(GeneratePopulation,
			ComputeFitness, MateFunction);

		mInitialized = true;
		mStop = false;
	}

	if (ImGui::Button("Stop"))
		mStop = true;
	ImGui::End();

	ImGui::SetNextWindowSize({ 800.0f, 600.0f });
	ImGui::Begin("PathFinding", nullptr, ImGuiWindowFlags_NoResize);
	ImVec2 winPos = ImGui::GetWindowPos();
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	ImU32 redColor = ImColor(255, 0, 0);
	ImU32 greenColor = ImColor(0, 255, 0);

	for (size_t y = 0; y < 19; ++y)
	{
		for (size_t x = 0; x < 26; ++x)
		{
			ImVec2 minPos{ 0.0f, 20.0f };
			ImVec2 maxPos{ x * 32.0f, y * 32.0f };
			drawList->AddRect(winPos + minPos, maxPos + winPos + minPos, greenColor);
		}
	}

	if (mInitialized)
	{
		auto& best = mGeneticAlgorithm.GetBestGenome();
		if (!mLast)
		{
			ImVec2 nextPos = mStartPos;
			for (auto direction : best.chromosome)
			{
				if (direction == 0 && mNextPos.y >= 30.0f)
					nextPos.y -= 30.0f;

				if (direction == 1 && mNextPos.y < 580.0f - 30.0f)
					nextPos.y += 30.0f;

				if (direction == 2 && mNextPos.x >= 30.0f)
					nextPos.x -= 30.0f;

				if (direction == 3 && mNextPos.y < 785.0f - 30.0f)
					nextPos.x += 30.0f;

				drawList->AddCircle(winPos + nextPos, 10.0f, redColor);
			}

			if (Distance({ mNextPos.x, mNextPos.y, 0.0f },
				{ mEndPos.x, mEndPos.y, 0.0f }) <= 0.0001f)
				mLast = true;
		}

	}

	drawList->AddCircle(winPos + mStartPos, 10.0f, redColor);
	drawList->AddCircle(winPos + mEndPos, 10.0f, redColor);
	ImGui::End();

	mAppLog.Draw("Console");
}