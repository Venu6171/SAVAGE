#pragma once
#include "Datasets.h"

namespace SAVAGE::ML
{
	class LinearRegression
	{
	public:
		float Fit(const Dataset<1>& dataset);
		float Predict(float X) const;

		float b0 = 0.0f;
		float b1 = 0.0f;
		float learningRate = 0.01f;
	};
}