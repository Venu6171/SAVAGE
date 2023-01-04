#pragma once

#include "Colors.h"

namespace SAVAGE::Graphics
{
	struct DirectionalLight
	{
		Math::Vector3 direction;
		float padding;
		Color ambient;
		Color diffuse;
		Color specular;
	};
}