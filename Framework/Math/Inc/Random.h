#pragma once
#include "EngineMath.h"

namespace SAVAGE::Math
{
	int RandomInt(int min = 0, int max = std::numeric_limits<int>::max());

	float RandomFloat(float min = 0.0f, float max = 1.0f);

	Vector2 RandomVector2(const Vector2& min, const Vector2& max);
	Vector3 RandomVector3(const Vector3& min, const Vector3& max);

}