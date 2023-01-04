#pragma once
#include "AnimationClip.h"

namespace SAVAGE::Graphics::AnimationIO
{
	void Write(FILE* file, const AnimationClip& animClip);
	void Read(FILE* file, AnimationClip& animClip);
}