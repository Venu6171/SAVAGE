#include "Precompiled.h"
#include "LinearRegression.h"

#include "Datasets.h"

using namespace SAVAGE::ML;
using namespace SAVAGE::Math;

float LinearRegression::Fit(const Dataset<1>& dataset)
{
	// Homework
	float error_b0 = 0.0f;
	float error_b1 = 0.0f;
	float mse = 0.0f;
	const float m = static_cast<float>(dataset.x.size());

	for (size_t i = 0; i < dataset.x.size(); ++i)
	{
		float predictedValue = Predict(dataset.x[i][0]);
		float trueValue = dataset.y[i];
		float error = predictedValue - trueValue;

		error_b0 += error;
		error_b1 += error * dataset.x[i][0];
		mse += Math::Sqr(error);

	}

	b0 -= learningRate * error_b0 / m;
	b1 -= learningRate * error_b1 / m;
	mse /= m;

	return mse;
}

float LinearRegression::Predict(float X) const
{
	return b0 + (b1 * X);
}
