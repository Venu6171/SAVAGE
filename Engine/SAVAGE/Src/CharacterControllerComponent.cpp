#include "Precompiled.h"

#include "AnimatorComponent.h"
#include "CharacterControllerComponent.h"
#include "GameWorld.h"
#include "HealthComponent.h"
#include "RenderService.h"
#include "TransformComponent.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;
MEMPOOL_DEFINE(CharacterControllerComponent, 1000)

void CharacterControllerComponent::Initialize()
{
	mRenderService = GetOwner().GetWorld().GetService<RenderService>();

	mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mHealthComponent = GetOwner().GetComponent<HealthComponent>();
}

void SAVAGE::CharacterControllerComponent::Update(float deltaTime)
{
	mAnimatorComponent->GetAnimator().Play(mPlayerState, mSetLooping, 1.0f);

	CharacterIdle(deltaTime);
	CharacterWalk(deltaTime);
	CharacterRun(deltaTime);
	CharacterAttack(deltaTime);

	auto& characterPosition = mTransformComponent->GetPosition();
	Vector3 cameraPosition = characterPosition + mCameraPositionOffset;
	mRenderService->GetCamera().SetPosition(cameraPosition);
	mRenderService->GetCamera().SetLookAt(characterPosition + Vector3(0.0f, 2.0f, 0.0f));

	const float turnSpeed = 5.0f * Constants::DegToRad;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		float angle = inputSystem->GetMouseMoveX() * turnSpeed * deltaTime;
		float pitch = inputSystem->GetMouseMoveY() * turnSpeed * deltaTime;

		float length = Math::Magnitude(mCameraPositionOffset);
		float pitchCurrent = asin(cameraPosition.y / length);
		float pitchLimit = -asin(2.0f / length);
		pitch = Clamp(pitch, pitchLimit - pitchCurrent, 35.0f * Math::Constants::DegToRad);

		mCameraPositionOffset = TransformNormal(mCameraPositionOffset, Matrix4::RotationY(angle));
		mCameraPositionOffset = TransformNormal(mCameraPositionOffset, Matrix4::RotationX(pitch));

		auto& rotation = mTransformComponent->GetRotation();
		auto rotateBy = Quaternion::RotationAxis(Math::Vector3::YAxis, angle);
		mTransformComponent->SetRotation(rotation * rotateBy);
	}
}

void CharacterControllerComponent::CharacterIdle(float deltaTime)
{
	mSetLooping = true;
	mAnimatorComponent->GetAnimator().SetClipIndex(Idle);
}

void CharacterControllerComponent::CharacterWalk(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	auto& position = mTransformComponent->GetPosition();
	auto& rotation = mTransformComponent->GetRotation();
	auto matRotation = Math::Matrix4::RotationQuaternion(rotation);
	auto directionX = Math::Vector3(matRotation._11, matRotation._12, matRotation._13);
	auto directionZ = Math::Vector3(matRotation._31, matRotation._32, matRotation._33);

	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mSetLooping = true;
		mAnimatorComponent->GetAnimator().SetClipIndex(WalkForward);
		mTransformComponent->SetPosition(position - (directionZ * mWalkSpeed * deltaTime));
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mSetLooping = true;
		mAnimatorComponent->GetAnimator().SetClipIndex(WalkBack);
		mTransformComponent->SetPosition(position + (directionZ * mWalkSpeed * deltaTime));
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mSetLooping = true;
		mAnimatorComponent->GetAnimator().SetClipIndex(WalkLeft);
		mTransformComponent->SetPosition(position + (directionX * mWalkSpeed * deltaTime));
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mSetLooping = true;
		mAnimatorComponent->GetAnimator().SetClipIndex(WalkRight);
		mTransformComponent->SetPosition(position - (directionX * mWalkSpeed * deltaTime));
	}
}

void CharacterControllerComponent::CharacterRun(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	auto& position = mTransformComponent->GetPosition();
	auto& rotation = mTransformComponent->GetRotation();
	auto matRotation = Math::Matrix4::RotationQuaternion(rotation);
	auto directionX = Math::Vector3(matRotation._11, matRotation._12, matRotation._13);
	auto directionZ = Math::Vector3(matRotation._31, matRotation._32, matRotation._33);

	if (inputSystem->IsKeyDown(KeyCode::W) && inputSystem->IsKeyDown(KeyCode::LSHIFT))
	{
		mSetLooping = true;
		mAnimatorComponent->GetAnimator().SetClipIndex(RunForward);
		mTransformComponent->SetPosition(position - (directionZ * mRunSpeed * deltaTime));
	}
	if (inputSystem->IsKeyDown(KeyCode::S) && inputSystem->IsKeyDown(KeyCode::LSHIFT))
	{
		mSetLooping = true;
		mAnimatorComponent->GetAnimator().SetClipIndex(RunBack);
		mTransformComponent->SetPosition(position + (directionZ * mRunSpeed * deltaTime));
	}
	if (inputSystem->IsKeyDown(KeyCode::A) && inputSystem->IsKeyDown(KeyCode::LSHIFT))
	{
		mSetLooping = true;
		mAnimatorComponent->GetAnimator().SetClipIndex(RunLeft);
		mTransformComponent->SetPosition(position + (directionX * mRunSpeed * deltaTime));
	}
	if (inputSystem->IsKeyDown(KeyCode::D) && inputSystem->IsKeyDown(KeyCode::LSHIFT))
	{
		mSetLooping = true;
		mAnimatorComponent->GetAnimator().SetClipIndex(RunRight);
		mTransformComponent->SetPosition(position - (directionX * mRunSpeed * deltaTime));
	}
}

void CharacterControllerComponent::CharacterAttack(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
	{
		mSetLooping = true;
		mAnimatorComponent->GetAnimator().SetClipIndex(Attack);
	}
}