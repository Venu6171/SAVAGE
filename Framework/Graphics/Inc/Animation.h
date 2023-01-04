#pragma once

#include "KeyFrame.h"

namespace SAVAGE::Graphics
{
	class Animation
	{
	public:
		Math::Vector3 GetPosition(float time) const;
		Math::Quaternion GetRotation(float time) const;
		Math::Vector3 GetScale(float time) const;

		Math::Matrix4 GetTransform(float time) const;

		PositionKeys& GetPositionKeys()
		{
			return mPositionKeys;
		}

		RotationKeys& GetRotationKeys()
		{
			return mRotationKeys;
		}

		PositionKeys& GetScaleKeys()
		{
			return mScaleKeys;
		}
	
	private:
		template <class T>
		int GetKeyFrameIndex(const std::vector<KeyFrame<T>>& container, float t) const;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		friend class AnimationBuilder;
		friend class Animator;
	};

	template <class T>
	int Animation::GetKeyFrameIndex(const KeyFrames<T>& container, float t) const
	{
		// [t = a]               [t = b]     [t = c]           [t = d]
		// f0                    f1          f2                f3

		//                                                                    ^
		// time = q

		//for (int i = 0; i < 4; ++i)
		//{
		//	currentFrame = container[i];
		//	nextFrame = container[i + 1];
		//	if (q > currentFrame.time && q < nextFrame.time)
		//	{
		//		index = i;
		//	}
		//}

		int index = -1;

		if (container.size() == 1)
		{
			return 0;
		}

		for (int i = 0; i < container.size() - 1; ++i)
		{
			// input t is at 1.5 .............V
			// you return index 0
			// but you should return index 1

			// input t is at 3.0 ..............................................V
			// you return index 0
			// but you should return index 2

			// [t = 0] ....................................... [t = 3] ............ [t = 4]
			// f0                                              f1                   f2
			// i = 0                                           i = 1                i = 2

			KeyFrame<T> currentFrame = container[i];
			KeyFrame<T> nextFrame = container[i + 1];

			if ((t >= currentFrame.time && t < nextFrame.time) || t == currentFrame.time)
			{
				index = i;
				break;
			}
			else if (i == container.size() - 2)
			{
				// time exceeded the last frame's time
				index = container.size() - 1;
			}
		}
		return index;
	}
}