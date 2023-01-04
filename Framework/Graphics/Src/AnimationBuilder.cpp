#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace SAVAGE::Graphics;

AnimationBuilder& SAVAGE::Graphics::AnimationBuilder::SetTime(float time)
{
	mTime = time;

	return *this;
}

AnimationBuilder& SAVAGE::Graphics::AnimationBuilder::AdvanceTime(float time)
{
	mTime += time;

	return *this;
}

AnimationBuilder& SAVAGE::Graphics::AnimationBuilder::AddPositionKey(const Math::Vector3& position)
{
	ASSERT(mTemp.mPositionKeys.empty() || mTemp.mPositionKeys.back().time < mTime, "PositionKey Time is less than the current time");
	mTemp.mPositionKeys.push_back({ position, mTime });

	return *this;
}

AnimationBuilder& SAVAGE::Graphics::AnimationBuilder::AddPositionKey(const Math::Vector3& position, float time)
{
	ASSERT(mTemp.mPositionKeys.empty() || mTemp.mPositionKeys.back().time < time, "PositionKey Time is less than the current time");
	mTemp.mPositionKeys.push_back({ position, time });

	return *this;
}

AnimationBuilder& SAVAGE::Graphics::AnimationBuilder::AddRotationKey(const Math::Quaternion& rotation)
{
	ASSERT(mTemp.mRotationKeys.empty() || mTemp.mRotationKeys.back().time < mTime, "RotationKey Time is less than the current time");
	mTemp.mRotationKeys.push_back({ rotation, mTime });

	return *this;
}

AnimationBuilder& SAVAGE::Graphics::AnimationBuilder::AddRotationKey(const Math::Quaternion& rotation, float time)
{
	ASSERT(mTemp.mRotationKeys.empty() ||mTemp.mRotationKeys.back().time < time, "RotationKey Time is less than the current time");
	mTemp.mRotationKeys.push_back({ rotation, time });

	return *this;
}

AnimationBuilder& SAVAGE::Graphics::AnimationBuilder::AddScaleKey(const Math::Vector3& scale)
{
	ASSERT(mTemp.mScaleKeys.empty() || mTemp.mScaleKeys.back().time < mTime, "ScaleKey Time is less than the current time");
	mTemp.mScaleKeys.push_back({ scale, mTime });

	return *this;
}

AnimationBuilder& SAVAGE::Graphics::AnimationBuilder::AddScaleKey(const Math::Vector3& scale, float time)
{
	ASSERT(mTemp.mScaleKeys.empty() || mTemp.mScaleKeys.back().time < time, "ScaleKey Time is less than the current time");
	mTemp.mScaleKeys.push_back({ scale, time });

	return *this;
}

//AnimationBuilder& SAVAGE::Graphics::AnimationBuilder::SetLooping(bool looping)
//{
//	mTemp.mLooping = looping;
//
//	return *this;
//}

Animation SAVAGE::Graphics::AnimationBuilder::Get()
{
	mTime = 0.0f;

	return std::move(mTemp);
}
