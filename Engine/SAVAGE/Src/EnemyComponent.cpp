#include "Precompiled.h"

#include "AnimatorComponent.h"
#include "EnemyComponent.h"
#include "GameWorld.h"
#include "HealthComponent.h"
#include "RenderService.h"
#include "TransformComponent.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;
using namespace SAVAGE::Math;
MEMPOOL_DEFINE(EnemyComponent, 1000)

void EnemyComponent::Initialize()
{
	mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mHealthComponent = GetOwner().GetComponent<HealthComponent>();

	mEnemyTransformComponent = GetOwner().GetWorld().FindGameObject("Viren")->GetComponent<TransformComponent>();
}

void EnemyComponent::Update(float deltaTime)
{
	mAnimatorComponent->GetAnimator().Play(mCurrentState, true, 1.0f);

	mTimer += deltaTime;

	switch (mCurrentState)
	{
	case Idle:
		UpdateIdle(deltaTime);
		break;
	case Walk:
		UpdateWalk(deltaTime);
		break;
	case Run:
		UpdateRun(deltaTime);
		break;
	case Attack:
		UpdateAttack(deltaTime);
		break;
	}

	if (mShowSphere)
		SimpleDraw::AddSphere(mTransformComponent->GetPosition(), mRadius, Colors::Red, 10, 10);
}

void EnemyComponent::UpdateIdle(float deltaTime)
{
	mIdleTimer += deltaTime;

	SetState(Idle);
	mAnimatorComponent->GetAnimator().SetClipIndex(Idle);
	SetRandomLocation(-50.0f, 50.0f, -50.0f, 50.0f);

	if (mIdleTimer > RandomFloat(5.0f, 15.0f))
	{
		auto random = RandomInt(1, 2);
		switch (random)
		{
		case 1:
			mCurrentState = Walk;
			break;
		case 2:
			mCurrentState = Run;
			break;
		}
		mIdleTimer = 0.0f;
	}
}

void EnemyComponent::UpdateWalk(float deltaTime)
{
	SetState(Walk);
	mAnimatorComponent->GetAnimator().SetClipIndex(Walk);
	IsPlayerNearby();

	Vector3 currentPosition = mTransformComponent->GetPosition();
	Vector3 positionToDestination;

	if (mPlayerNearby)
	{
		positionToDestination = mEnemyTransformComponent->GetPosition() - currentPosition;
	}
	else
	{
		positionToDestination = mNextPosition - currentPosition;
	}

	float distance = Magnitude(positionToDestination);
	auto direction = Normalize(positionToDestination);

	if (distance > 1.0f)
	{
		currentPosition += direction * mWalkSpeed * deltaTime;
		mTransformComponent->SetPosition(currentPosition);
		CheckDirection();
	}
	else
		mCurrentState = Idle;
}

void EnemyComponent::UpdateRun(float deltaTime)
{
	SetState(Run);
	mAnimatorComponent->GetAnimator().SetClipIndex(Run);
	IsPlayerNearby();

	Vector3 currentPosition = mTransformComponent->GetPosition();
	Vector3 positionToDestination;

	if (mPlayerNearby)
	{
		positionToDestination = mEnemyTransformComponent->GetPosition() - currentPosition;
	}
	else
	{
		positionToDestination = mNextPosition - currentPosition;
	}

	float distance = Magnitude(positionToDestination);
	auto direction = Normalize(positionToDestination);

	if (distance > 1.0f)
	{
		currentPosition += direction * mRunSpeed * deltaTime;
		mTransformComponent->SetPosition(currentPosition);

		CheckDirection();
	}
	else
		mCurrentState = Idle;
}

void EnemyComponent::UpdateAttack(float deltaTime)
{
	mAnimatorComponent->GetAnimator().SetClipIndex(Attack);
	IsPlayerNearby();
	CheckDirection();
}

Vector3 EnemyComponent::SetRandomLocation(float minX, float maxX, float minZ, float maxZ)
{
	mNextPosition = RandomVector3({ minX, 0.0f, minZ }, { maxX, 0.0f, maxZ });

	return mNextPosition;
}

void EnemyComponent::IsPlayerNearby()
{
	auto& currentPosition = mTransformComponent->GetPosition();
	auto playerToEnemy = currentPosition - mEnemyTransformComponent->GetPosition();
	float distance = Magnitude(playerToEnemy);

	if (distance <= 2.0f)
		mCurrentState = Attack;
	else if (distance <= 6.0f && distance >= 2.0f)
		mPlayerNearby = true;
	else if (distance >= 20.0f)
		mPlayerNearby = false;
	else
		mCurrentState = GetLastState();
}

void EnemyComponent::CheckDirection()
{
	auto& currentPosition = mTransformComponent->GetPosition();
	Vector3 distance;
	if (mPlayerNearby)
	{
		distance = mEnemyTransformComponent->GetPosition() - currentPosition;
	}
	else
	{
		distance = mNextPosition - currentPosition;
	}
	auto direction = Normalize(distance);

	mTransformComponent->SetRotation(Quaternion::RotationLook(-(direction * mTimer)));
}