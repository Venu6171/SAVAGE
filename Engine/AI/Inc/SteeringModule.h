#pragma once

#include "SteeringBehavior.h"

namespace SAVAGE::AI
{
	class SteeringModule
	{
	public:
		SteeringModule(Agent& agent);

		template <class BehaviorType>
		BehaviorType* AddBehavior();

		SAVAGE::Math::Vector2 Calculate();

	private:
		using Behaviors = std::vector<std::unique_ptr<SteeringBehavior>>;

		Agent& mAgent;
		Behaviors mBehaviors;
	};

	template <class BehaviorType>
	BehaviorType* SteeringModule::AddBehavior()
	{
		static_assert(std::is_base_of_v<SteeringBehavior, BehaviorType>,
			"BehaviorType is not a child of AI::SteeringBehavior!");

		auto& newBehavior = mBehaviors.emplace_back(std::make_unique<BehaviorType>());
		return static_cast<BehaviorType*>(newBehavior.get());
	}
}