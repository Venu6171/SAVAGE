#pragma once

#include "Datasets.h"

namespace SAVAGE::ML
{
	class LogisticRegression
	{
	public:
		float Fit(const Dataset<2>& dataset);
		float Predict(float x0, float x1) const;

		float b0 = 0.0f;
		float b1 = 0.0f;
		float b2 = 0.0f;
		float learningRate = 0.01f;
	};
}