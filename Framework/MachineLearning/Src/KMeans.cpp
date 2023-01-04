#include "Precompiled.h"
//#include "KMeans.h"
//
//#include "Datasets.h"
//
//using namespace SAVAGE::ML;
//using namespace SAVAGE::Math;
//
//float SAVAGE::ML::KMeans::Fit(const Dataset<2>& dataset)
//{
//
//	//ASSERT(k <= dataset.size(), "Not Enough data for %zu clusters!", k);
//
//	centroids.resize(dataset.x[0].size());
//	assignments.resize(dataset.x[0].size());
//	//clusterResult.centroidHistory.resize(k);
//
//	// Generate random starting points
//	/*for (auto& centroid : clusterResult.centroids)
//	{
//		centroid.x = RandomFloat(0.0f, 800.0f);
//		centroid.y = RandomFloat(0.0f, 600.0f);
//	}*/
//
//	// Use existing data points as the starting cluster positions
//	for (size_t i = 0; i < k; ++i)
//		for (size_t j = 0; j < k; ++j)
//			centroids[i][j] = dataset.x[0][i];
//
//	for (size_t iter = 0; iter < maxIteration; ++iter)
//	{
//		for (size_t i = 0; i < dataset.x[0].size(); ++i)
//		{
//			float closestDistance = FLT_MAX;
//			for (size_t j = 0; j < centroids.size(); ++j)
//			{
//				float distance = DistanceSqr(centroids[j], dataset.x[0][i]);
//				if (closestDistance > distance)
//				{
//					closestDistance = distance;
//					assignments[i] = j;
//				}
//			}
//		}
//
//		std::vector<std::array<float, 2>> newCentroids;
//		newCentroids.resize(k);
//		int count = 0;
//
//		for (size_t i = 0; i < k; ++i)
//		{
//			count = 0;
//			for (size_t j = 0; j < assignments.size(); ++j)
//			{
//				if (assignments[j] == i)
//				{
//					newCentroids[i] += dataset.x[i][j];
//					count++;
//				}
//			}
//			newCentroids[i] /= count;
//		}
//
//		//for (size_t i = 0; i < clusterResult.centroidHistory.size(); ++i)
//		//	clusterResult.centroidHistory[i].push_back(clusterResult.centroids[i]);
//
//		for (size_t i = 0; i < newCentroids.size(); ++i)
//		{
//			if (newCentroids[i] == centroids[i])
//				break;
//			else
//				centroids = std::move(newCentroids);
//		}
//	}
//
//	return clusterResult;
//}
//
//size_t SAVAGE::ML::KMeans::Predict(float x0, float x1) const
//{
//	return size_t();
//}