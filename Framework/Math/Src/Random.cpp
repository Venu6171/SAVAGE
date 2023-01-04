#include "Precompiled.h"
#include "Random.h"

using namespace SAVAGE;

namespace
{
	std::random_device mRandomDevice{};
	std::mt19937 randomEngine{ mRandomDevice() };
}

int Math::RandomInt(int min, int max)
{
	return std::uniform_int_distribution<>{ min, max }(randomEngine);
}

float Math::RandomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>{min, max}(randomEngine);
}

Math::Vector2 Math::RandomVector2(const Vector2& min, const Vector2& max)
{
	return Math::Vector2
	(
		RandomFloat(min.x, max.x), // x
		RandomFloat(min.y, max.y)  // y
	);
}

Math::Vector3 SAVAGE::Math::RandomVector3(const Vector3& min, const Vector3& max)
{
	return Math::Vector3
	(
		RandomFloat(min.x, max.x), // x
		RandomFloat(min.y, max.y), // y
		RandomFloat(min.z, max.z)  // z
	);
}
