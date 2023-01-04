#pragma once

#include "Animation.h"

namespace SAVAGE::Graphics
{
	struct AnimationClip
	{
		std::string name;
		float duration = 0.0f;
		float ticksPerSecond = 0.0f;
		bool mLooping = false;
		std::vector<std::unique_ptr<Animation>> boneAnimations;
	};
}