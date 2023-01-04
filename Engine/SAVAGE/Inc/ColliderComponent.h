#pragma once

#include "Component.h"

namespace SAVAGE
{
	class TransformComponent;

	class ColliderComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Collider);
		MEMPOOL_DECLARE;

		ColliderComponent() = default;

		void Initialize() override;
		void DebugUI() override;

		Math::AABB GetAABB() const;

		Math::Vector3 SetCenter(const Math::Vector3& center) { return mCenter = center; }
		Math::Vector3 SetExtend(const Math::Vector3& extend) { return mExtend = extend; }

		void SetShowCollider(bool collider) { mShowCollider = collider; }
		bool GetCollider() { return mShowCollider; }
	private:
		const TransformComponent* mTransformComponent = nullptr;
		Math::Vector3 mCenter = Math::Vector3::Zero;
		Math::Vector3 mExtend = Math::Vector3::One;

		bool mShowCollider = false;
	};
}