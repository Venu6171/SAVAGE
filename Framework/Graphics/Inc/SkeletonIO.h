#pragma once
#include "Skeleton.h"

namespace SAVAGE::Graphics::SkeletonIO
{
	void Write(FILE* file, const Skeleton& skeleton);
	void Read(FILE* file, Bone& bone);
}