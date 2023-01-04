#pragma once

#include "Component.h"

namespace SAVAGE
{
	class TransformComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::TransformId);
		MEMPOOL_DECLARE;

		TransformComponent() = default;

		void DebugUI() override;

		void SetPosition(const Math::Vector3& position) { mPosition = position; }
		const Math::Vector3& GetPosition() const { return mPosition; }

		void SetRotation(const Math::Quaternion& rotation) { mRotation = rotation; }
		const Math::Quaternion& GetRotation() const { return mRotation; }

		void SetScale(const Math::Vector3& scale) { mScale = scale; }
		const Math::Vector3& GetScale() const { return mScale; }

		void SetShowTransform(bool showTransform) { mShowTransform = showTransform; }
		bool GetTransform() { return mShowTransform; }
	private:
		Math::Vector3 mPosition;
		Math::Quaternion mRotation;
		Math::Vector3 mScale;

		bool mShowTransform = false;
	};
}