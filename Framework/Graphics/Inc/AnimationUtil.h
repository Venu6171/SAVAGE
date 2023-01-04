#pragma once

namespace SAVAGE::Graphics
{
	struct AnimationClip;
	struct Skeleton;

	void DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices);
	void GetToRootTransform(Skeleton& skeleton, std::vector<Math::Matrix4>& toRootTransform);
	void GetAnimationTransform(const Skeleton& skeleton, const AnimationClip& animClip, float time, std::vector<Math::Matrix4>& toRootTransform);
}