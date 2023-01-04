#pragma once

#include "Component.h"

namespace SAVAGE
{
	class ModelComponent;

	class HealthComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Health);
		MEMPOOL_DECLARE;

		HealthComponent() = default;

		void Initialize() override;

		void Update(float deltaTime) override;

		float SetHealth(float health) { return mHealth = health; }
		float GetHealth() { return mHealth; }

	private:

		ModelComponent* mModelComponent = nullptr;

		float mHealth = 0.0f;
	};
}