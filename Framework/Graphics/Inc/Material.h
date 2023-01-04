#pragma once

#include "Colors.h"

namespace SAVAGE::Graphics
{
	struct Material
	{
		Color ambient;
		Color diffuse;
		Color specular;
		float power = 1.0f;
		float padding[3];
	};
}