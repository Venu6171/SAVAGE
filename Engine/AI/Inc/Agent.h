#pragma once

#include "Entity.h"

namespace SAVAGE::AI
{
	class Agent;
	using Agents = std::vector<Agent*>;

	class Agent : public Entity
	{
	public:
		Agent(AIWorld& world, uint16_t typeId)
			: Entity(world, typeId)
		{}

		Agents neighbors;
		SAVAGE::Math::Vector2 velocity = SAVAGE::Math::Vector2::Zero;
		SAVAGE::Math::Vector2 destination = SAVAGE::Math::Vector2::Zero;
		float mSpeed = 1.0f;
		float mass = 1.0f;

	};

	using Entities = std::vector<Entity*>;
}
