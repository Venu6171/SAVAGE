#include "Precompiled.h"
#include "DataSets.h"

using namespace SAVAGE::ML;

namespace
{
	std::random_device sRandomDevice{};
	std::mt19937 sRandomEngine{ sRandomDevice() };

	float RandomNormal(float mean, float stdev)
	{
		return std::normal_distribution<float>{mean, stdev}(sRandomEngine);
	}

	float RandomUniform(float min, float max)
	{
		return std::uniform_real_distribution<float>{min, max}(sRandomEngine);
	}
}

Dataset<1> Datasets::MakeLinear(size_t n_Samples, float b0, float b1, float minX, float maxX, float noise)
{
	Dataset<1> dataSet;
	dataSet.x.reserve(n_Samples);
	dataSet.y.reserve(n_Samples);
	for (size_t i = 0; i < n_Samples; ++i)
	{
		float x = RandomUniform(minX, maxX);
		float y = b0 + (b1 * x) + RandomNormal(0.0f, noise);
		dataSet.x.push_back({ x });
		dataSet.y.push_back(y);
	}

	return dataSet;
}

Dataset<2> Datasets::MakeLogistic(size_t n_Samples, float b0, float b1, float b2, float minX, float maxX, float noise)
{
	Dataset<2> dataSet;
	dataSet.x.reserve(n_Samples);
	dataSet.y.reserve(n_Samples);

	const float m = -b1 / b2;
	const float b = -b0 / b2;

	for (size_t i = 0; i < n_Samples; ++i)
	{
		float x0 = RandomUniform(minX, maxX);
		float point = b + (m * x0);
		float delta = RandomNormal(0.0f, noise);
		float x1 = point + delta;
		float y = (delta > 0.0f) ? 1.0f : 0.0f;

		dataSet.x.push_back({ x0, x1 });
		dataSet.y.push_back(y);
	}

	return dataSet;
}