#pragma once
#include "Common.h"

namespace SAVAGE::Graphics
{
	template <class T>
	struct KeyFrame
	{
		T key;
		float time = 0.0f;
	};

	template<class T>
	using KeyFrames = std::vector<KeyFrame<T>>;

	using PositionKeys = KeyFrames<Math::Vector3>;
	using RotationKeys = KeyFrames<Math::Quaternion>;
	using ScaleKeys = KeyFrames<Math::Vector3>;
}
