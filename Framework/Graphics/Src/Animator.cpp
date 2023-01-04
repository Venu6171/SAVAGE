#include "Precompiled.h"
#include "Animator.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;

void Animator::Initialize(const Model* model)
{
	mModel = model;
}

void Animator::Terminate()
{
	mModel = nullptr;
	mAnimationTimer = 0.0f;
}

void Animator::Play(int clipIndex, bool looping, float animationSpeed)
{
	mAnimate = true;
	mLooping = looping;
	mAnimClipIndex = clipIndex;
	mAnimationSpeed = animationSpeed;
}

void Animator::Stop()
{
	mAnimate = false;
}

void Animator::Update(float deltaTime)
{
	if (mAnimate)
	{
		auto& animationClip = mModel->animSet[mAnimClipIndex];
		mAnimationTimer += animationClip->ticksPerSecond * deltaTime * mAnimationSpeed;
		while (mLooping && mAnimationTimer > animationClip->duration)
			mAnimationTimer -= animationClip->duration;

		GetAnimationTransform(*mModel->skeleton, *mModel->animSet[mAnimClipIndex], mAnimationTimer, mSkeletonTransforms);
	}
	else
		GetToRootTransform(*mModel->skeleton, mSkeletonTransforms);

	mToLocalTransforms.resize(mSkeletonTransforms.size());
	for (auto& bone : mModel->skeleton->bones)
	{
		mToLocalTransforms[bone->index] = bone->offsetTransform * mSkeletonTransforms[bone->index];
	}
}

const std::vector<Math::Matrix4>& Animator::GetSkeletonTrasforms() const
{
	// TODO: insert return statement here
	return mSkeletonTransforms;
}

const std::vector<Math::Matrix4>& Animator::GetToLocalTransforms() const
{
	// TODO: insert return statement here
	return mToLocalTransforms;
}