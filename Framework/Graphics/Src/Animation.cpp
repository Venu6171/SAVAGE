#include "Precompiled.h"
#include "Animation.h"

using namespace SAVAGE::Graphics;

SAVAGE::Math::Vector3 Animation::GetPosition(float time) const
{
	int index = GetKeyFrameIndex(mPositionKeys, time);

	Math::Vector3 currentPos{ 0.0f, 0.0f, 0.0f };

	if (index + 1 < mPositionKeys.size())
	{
		if (mPositionKeys[index].time != 0.0f)
		{
			time = fmod(time, mPositionKeys[index].time);
		}
		currentPos = Math::Lerp(mPositionKeys[index].key, mPositionKeys[index + 1].key, Math::Clamp(time / (mPositionKeys[index + 1].time - mPositionKeys[index].time), 0.0f, 1.0f));
	}
	else
	{
		currentPos = mPositionKeys[index].key;
	}

	return currentPos;
}

SAVAGE::Math::Quaternion Animation::GetRotation(float time) const
{
	int index = GetKeyFrameIndex(mRotationKeys, time);

	Math::Quaternion currentPos{ 1.0f, 0.0f, 0.0f, 0.0f };

	if (index + 1 < mRotationKeys.size())
	{
		if (mRotationKeys[index].time != 0.0f)
		{
			time = fmod(time, mRotationKeys[index].time);
		}
		currentPos = Math::Slerp(mRotationKeys[index].key, mRotationKeys[index + 1].key, Math::Clamp(time / (mRotationKeys[index + 1].time - mRotationKeys[index].time), 0.0f, 1.0f));
	}
	else
	{
		currentPos = mRotationKeys[index].key;
	}

	return currentPos;
}

SAVAGE::Math::Vector3 Animation::GetScale(float time) const
{
	int index = GetKeyFrameIndex(mScaleKeys, time);

	Math::Vector3 currentPos{ 0.0f, 0.0f, 0.0f };

	if (index + 1 < mScaleKeys.size())
	{
		if (mScaleKeys[index].time != 0.0f)
		{
			time = fmod(time, mScaleKeys[index].time);
		}
		currentPos = Math::Lerp(mScaleKeys[index].key, mScaleKeys[index + 1].key, Math::Clamp(time / (mScaleKeys[index + 1].time - mScaleKeys[index].time), 0.0f, 1.0f));
	}
	else
	{
		currentPos = mScaleKeys[index].key;
	}

	return currentPos;
}

SAVAGE::Math::Matrix4 SAVAGE::Graphics::Animation::GetTransform(float time) const
{
	auto pos = GetPosition(time);
	auto rot = GetRotation(time);
	auto scale = GetScale(time);

	Math::Matrix4 transform =
		Math::Matrix4::Scaling(scale) *
		Math::Matrix4::RotationQuaternion(Normalize(rot)) *
		Math::Matrix4::Translation(pos);

	return transform;
}
