#pragma once

#include "Model.h"
#include "AnimationUtil.h"

namespace SAVAGE::Graphics
{
	class Animator
	{
	public:

		void Initialize(const Model* model);
		void Terminate();

		void Play(int clipIndex, bool looping, float animationnSpeed);
		void Stop();

		void Update(float deltaTime);

		const std::vector<Math::Matrix4>& GetSkeletonTrasforms() const;
		const std::vector<Math::Matrix4>& GetToLocalTransforms() const;

		void SetClipIndex(int clipIndex) { mAnimClipIndex = clipIndex; }
	private:
		const Model* mModel = nullptr;
		std::vector<Math::Matrix4> mSkeletonTransforms;
		std::vector<Math::Matrix4> mToLocalTransforms;
		float mAnimationTimer = 0.0f;
		int mAnimClipIndex = 0;
		bool mAnimate = false;
		bool mLooping = false;
		float mAnimationSpeed = 1.0f;
	};
}