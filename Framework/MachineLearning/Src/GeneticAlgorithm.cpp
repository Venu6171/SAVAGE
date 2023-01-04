#include "Precompiled.h"
#include "GeneticAlgorithm.h"

using namespace SAVAGE::Math;
using namespace SAVAGE::ML;

void GeneticAlgorithm::Initialize(GeneratePopulation generatePopulation,
	ComputeFitnessFunc computeFitness, MateFunction mateFunction)
{
	mComputeFitness = std::move(computeFitness);
	mMateFunction = std::move(mateFunction);

	// Reset
	mGeneration = 0;

	// Create initial population
	mPopulation.clear();
	mPopulation = std::move(generatePopulation());
	EvaluatePopulation();
}

void GeneticAlgorithm::Advance()
{
	++mGeneration;

	std::vector<Genome> newPopulation;
	newPopulation.reserve(mPopulation.size());

	// Pick only from the better group
	// Perform Elitism: Keep top 10%, mate the rest using top 50%
	const int eliteCount = static_cast<int>(mPopulation.size() * 10) / 100;

	for (int i = 0; i < eliteCount; ++i)
		newPopulation.push_back(mPopulation[i]);

	const int mateCount = static_cast<int>(mPopulation.size()) - eliteCount;
	const int cutoff = static_cast<int>(mPopulation.size()) / 2;

	for (int i = 0; i < mateCount; ++i)
	{
		const auto& parent1 = mPopulation[RandomInt(0, cutoff)];
		const auto& parent2 = mPopulation[RandomInt(0, cutoff)];
		newPopulation.push_back(mMateFunction(parent1, parent2));
	}

	mPopulation = std::move(newPopulation);
	EvaluatePopulation();
}

void GeneticAlgorithm::EvaluatePopulation()
{
	for (auto& genome : mPopulation)
		genome.fitness = mComputeFitness(genome);

	std::sort(mPopulation.begin(), mPopulation.end(), [](const auto& a, const auto& b)
	{
		return a.fitness > b.fitness;
	});
}
