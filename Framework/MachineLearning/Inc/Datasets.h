#pragma once

namespace SAVAGE::ML
{
	template <size_t T>
	struct Dataset
	{
		std::vector<std::array<float, T>> x;
		std::vector<float> y;
	};

	namespace Datasets
	{
		// here b0 is y-intercept and b1 is the slope
		Dataset<1> MakeLinear(size_t n_Samples, float b0, float b1, float minX, float maxX, float noise);

		// here y-intercept is -(b0/b2) and slope is -(b1/b2)
		Dataset<2> MakeLogistic(size_t n_Samples, float b0, float b1, float b2, float minX, float maxX, float noise);
	}
}