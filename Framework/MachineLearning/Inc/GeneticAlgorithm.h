#pragma once

namespace SAVAGE::ML
{
	class GeneticAlgorithm
	{
	public:
		// Genome is a set of chromosome that represents a particular creature
		// Genome -> Chromosomes -> Genes
		struct Genome
		{
			std::vector<int> chromosome; // [gene][gene][gene][gene]......
			float fitness = 0.0f;
		};

		using GeneratePopulation = std::function<std::vector<Genome>()>;
		using ComputeFitnessFunc = std::function<float(const Genome&)>;
		using MateFunction = std::function<Genome(const Genome&, const Genome&)>;

		void Initialize(GeneratePopulation generatePopulation,
			ComputeFitnessFunc computeFitness,
			MateFunction mateFunction);
		void Advance();

		const Genome& GetBestGenome() const { return mPopulation.front(); };
		int GetGeneration() const { return mGeneration; };

	private:
		void EvaluatePopulation();

		ComputeFitnessFunc mComputeFitness;
		MateFunction mMateFunction;

		std::vector<Genome> mPopulation;
		int mGeneration = 0;
		int mMaxGeneValue = 0;
	};
}