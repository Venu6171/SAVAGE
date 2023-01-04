#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace SAVAGE;

MEMPOOL_DEFINE(ColliderComponent, 1000);

void ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void SAVAGE::ColliderComponent::DebugUI()
{
	const auto aabb = GetAABB();
	if (mShowCollider)
	{
		Graphics::SimpleDraw::AddAABB3D(aabb.center, aabb.extend, Graphics::Colors::Red);
	}
}

Math::AABB SAVAGE::ColliderComponent::GetAABB() const
{
	Math::AABB aabb;
	aabb.center = mTransformComponent->GetPosition() + mCenter;
	aabb.extend = mExtend;
	return aabb;
}
