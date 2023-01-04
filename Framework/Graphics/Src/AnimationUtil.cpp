#include "Precompiled.h"
#include "AnimationUtil.h"
#include "AnimationClip.h"
#include "Skeleton.h"
#include "SimpleDraw.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;

void DrawBone(Bone* bone, const std::vector<Math::Matrix4>& boneMatrices)
{
	Math::Vector3 bonePosition = Math::Vector3(boneMatrices[bone->index]._41, boneMatrices[bone->index]._42, boneMatrices[bone->index]._43);
	for (auto& child : bone->children)
	{
		Math::Vector3 boneChildrenPosition = Math::Vector3(boneMatrices[child->index]._41, boneMatrices[child->index]._42, boneMatrices[child->index]._43);
		SimpleDraw::AddLine3D(bonePosition, boneChildrenPosition, Colors::Blue);
		DrawBone(child, boneMatrices);
	}
}

void GetToRootTransformRecursive(const Bone* bone, std::vector<Math::Matrix4>& toRootTransform)
{
	if (bone->parent)
	{
		toRootTransform[bone->index] = bone->toParentTransform * toRootTransform[bone->parentIndex];
	}

	for (uint32_t i = 0; i < bone->children.size(); ++i)
	{
		GetToRootTransformRecursive(bone->children[i], toRootTransform);
	}
}

void GetAnimationTransformRecursive(const Bone* bone, const AnimationClip& animClip, float time, std::vector<Math::Matrix4>& toRootTransform)
{
	if (bone->parent)
	{
		auto& boneAnimation = animClip.boneAnimations[bone->index];
		if (boneAnimation != nullptr)
		{
			if (!boneAnimation->GetPositionKeys().empty() && !boneAnimation->GetRotationKeys().empty() && !boneAnimation->GetScaleKeys().empty())
			{
				toRootTransform[bone->index] = boneAnimation->GetTransform(time) * toRootTransform[bone->parentIndex];
			}
			else
			{
				toRootTransform[bone->index] = bone->toParentTransform * toRootTransform[bone->parentIndex];
			}
		}
		else
		{
			toRootTransform[bone->index] = bone->toParentTransform * toRootTransform[bone->parentIndex];
		}
	}
	else
	{
		toRootTransform[bone->index] = bone->toParentTransform;
	}

	for (uint32_t i = 0; i < bone->children.size(); ++i)
	{
		GetAnimationTransformRecursive(bone->children[i], animClip, time, toRootTransform);
	}
}

void SAVAGE::Graphics::DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices)
{
	for (int i = 0; i < skeleton.bones.size(); ++i)
	{
		Math::Vector3 jointPosition = Math::Vector3(boneMatrices[i]._41, boneMatrices[i]._42, boneMatrices[i]._43);
		SimpleDraw::AddSphere(jointPosition, 0.01f, Colors::Red, 5.0f, 5.0f);
	}
	DrawBone(skeleton.root, boneMatrices);
}

void SAVAGE::Graphics::GetToRootTransform(Skeleton& skeleton, std::vector<Math::Matrix4>& toRootTransform)
{
	toRootTransform.resize(skeleton.bones.size());
	GetToRootTransformRecursive(skeleton.root, toRootTransform);
}

void SAVAGE::Graphics::GetAnimationTransform(const Skeleton& skeleton, const AnimationClip& animClip, float time, std::vector<Math::Matrix4>& toRootTransform)
{
	toRootTransform.resize(skeleton.bones.size());
	GetAnimationTransformRecursive(skeleton.root, animClip, time, toRootTransform);
}