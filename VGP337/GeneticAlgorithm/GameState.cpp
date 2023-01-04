#include "GameState.h"
#include <fstream>
#include <sstream>

using namespace SAVAGE;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;
using namespace SAVAGE::ML;

namespace
{
	const std::string kTarget = "Genetic Algorithm is Working!";
	const int kValidGeneValue = 126 - 32; // from character '~' to ' ' from ASCII table
	int mPopulationSize = 100;
	float mMutationRate = 0.1f;
	float mCrossOverRate = 0.45f;

}

void GameState::Initialize()
{
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

		if (best.fitness < static_cast<int>(kTarget.size()))
		{
			mGeneticAlgorithm.Advance();

			// Print the next Best Genome
			std::string bestStr;
			for (auto& gene : mGeneticAlgorithm.GetBestGenome().chromosome)
				bestStr += static_cast<char>(gene + 32);
			mAppLog.AddLog("Generation[%d] %s Fitness: %f\n", mGeneticAlgorithm.GetGeneration(), bestStr.c_str(), mGeneticAlgorithm.GetBestGenome().fitness);
		}
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Values", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::InputFloat("MutationRate", &mMutationRate, 0.1f, 1.0f, "%0.2f");
	ImGui::InputFloat("CrossOverRate", &mCrossOverRate, 0.1f, 1.0f, "%0.2f");

	if (ImGui::Button("Run"))
	{
		auto GeneratePopulation = []()
		{
			std::vector<GeneticAlgorithm::Genome> mPopulation;
			const size_t chromoLength = kTarget.size();
			mPopulation.resize(mPopulationSize);
			for (auto& genome : mPopulation)
			{
				genome.chromosome.reserve(chromoLength);
				for (size_t i = 0; i < chromoLength; ++i)
					genome.chromosome.push_back(RandomInt(0, kValidGeneValue));
			}

			return mPopulation;
		};
		auto ComputeFitness = [](auto& genome)
		{
			float fitness = 0.0f;

			// One point per matching character
			for (size_t i = 0; i < kTarget.size(); ++i)
			{
				if (kTarget[i] == genome.chromosome[i] + 32)
					fitness += 1.0f;
			}

			return fitness;
		};
		auto MateFunction = [](const auto& parent1, const auto& parent2)
		{
			GeneticAlgorithm::Genome offspring;
			offspring.chromosome.reserve(parent1.chromosome.size());

			for (size_t i = 0; i < parent1.chromosome.size(); ++i)
			{
				if (RandomFloat() < mMutationRate)
					offspring.chromosome.push_back(RandomInt(0, kValidGeneValue));
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

	mAppLog.Draw("Console");
}