#pragma once

#include "Component.h"

namespace SAVAGE
{
	class AnimatorComponent;
	class TransformComponent;
	class HealthComponent;

	class EnemyComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::EnemyController);
		MEMPOOL_DECLARE;

		EnemyComponent() = default;

		void Initialize() override;

		void Update(float deltaTime) override;

		void SetShowSphere(bool showSphere) { mShowSphere = showSphere; }
		void SetWalkSpeed(float speed) { mWalkSpeed = speed; }
		void SetRunSpeed(float speed) { mRunSpeed = speed; }

	private:
		AnimatorComponent* mAnimatorComponent = nullptr;
		TransformComponent* mTransformComponent = nullptr;
		TransformComponent* mEnemyTransformComponent = nullptr;
		HealthComponent* mHealthComponent = nullptr;

		enum State
		{
			Idle = 0,
			Walk = 1,
			Run = 2,
			Attack = 3
		};

		State mCurrentState = State::Idle;
		State mLastState;

		State SetState(State mState) { return mLastState = mState; }
		State GetLastState() { return mLastState; }

		void UpdateIdle(float deltaTime);
		void UpdateWalk(float deltaTime);
		void UpdateRun(float deltaTime);
		void UpdateAttack(float deltaTime);

		void IsPlayerNearby();
		void CheckDirection();

		SAVAGE::Math::Vector3 mNextPosition;

		SAVAGE::Math::Vector3 SetRandomLocation(float minX, float maxX, float minZ, float maxZ);
		SAVAGE::Math::Vector3 GetRandomLocation() { return mNextPosition; }

		float mWalkSpeed = 1.5f;
		float mRunSpeed = 3.0f;
		float mIdleTimer = 0.0f;
		float mTimer = 0.0f;
		float mRadius = 3.0f;

		bool mPlayerNearby = false;
		bool mShowSphere = false;
	};
}