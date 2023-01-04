#pragma once

#include <Math\Inc\EngineMath.h>
#include "Datasets.h"

namespace SAVAGE::ML
{
	class KMeans
	{
	public:
		KMeans(size_t numCluster, size_t maxIter)
		{
			k = numCluster;
			maxIteration = maxIter;
		}

		float Fit(const Dataset<2>& dataset);
		size_t Predict(float x0, float x1) const;

		std::vector<std::array<float, 2>> centroids;
		std::vector<size_t> assignments;

		std::vector<std::vector<float>> centroidHistory;

	private:
		size_t maxIteration;
		size_t k;
	};
}