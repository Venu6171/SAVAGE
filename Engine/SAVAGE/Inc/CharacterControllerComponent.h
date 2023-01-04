#pragma once

#include "Component.h"

enum PlayerState
{
	Idle = 0,
	WalkForward = 1,
	WalkLeft = 2,
	WalkRight = 3,
	WalkBack = 4,
	RunForward = 5,
	RunLeft = 6,
	RunRight = 7,
	RunBack = 8,
	Attack = 9,
	Jump = 10
};

namespace SAVAGE
{
	class AnimatorComponent;
	class HealthComponent;
	class RenderService;
	class TransformComponent;

	class CharacterControllerComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Controller);
		MEMPOOL_DECLARE;

		CharacterControllerComponent() = default;

		void Initialize() override;

		void Update(float deltaTime) override;

	private:
		PlayerState mPlayerState;

		void CharacterIdle(float deltaTime);
		void CharacterWalk(float deltaTime);
		void CharacterRun(float deltaTime);
		void CharacterAttack(float deltaTime);

		AnimatorComponent* mAnimatorComponent = nullptr;
		HealthComponent* mHealthComponent = nullptr;
		RenderService* mRenderService = nullptr;
		TransformComponent* mTransformComponent = nullptr;

		SAVAGE::Math::Vector3 mCameraPositionOffset = { 0.0f, 2.5f, 5.0f };

		bool mAnimationFinished = false;
		bool mAnimationPlaying = false;
		float mWalkSpeed = 1.5f;
		float mRunSpeed = 3.5f;
		bool mAttacking = false;
		bool mSetLooping = false;
	};
}