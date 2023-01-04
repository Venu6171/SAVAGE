#include "Precompiled.h"
#include "LogisticRegression.h"

#include "Datasets.h"

using namespace SAVAGE::ML;

float LogisticRegression::Fit(const Dataset<2>& dataset)
{
	float db0 = 0.0f;
	float db1 = 0.0f;
	float db2 = 0.0f;
	float mse = 0.0f;

	for (size_t i = 0; i < dataset.x.size(); ++i)
	{
		float prediction = Predict(dataset.x[i][0], dataset.x[i][1]);
		float error = prediction - dataset.y[i];
		db0 += error;
		db1 += error * dataset.x[i][0];
		db2 += error * dataset.x[i][1];
		mse += sqrt(error);
	}

	const float m = static_cast<float>(dataset.x.size());
	b0 -= learningRate * db0 / m;
	b1 -= learningRate * db1 / m;
	b2 -= learningRate * db2 / m;

	return mse / m;
}

float LogisticRegression::Predict(float x0, float x1) const
{
	const float z = b0 + (b1 * x0) + (b2 * x1);
	return 1.0f / (1.0f + exp(-z));
}
