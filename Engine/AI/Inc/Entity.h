#pragma once

#include "Common.h"

namespace SAVAGE::AI
{
	class Entity;
	class AIWorld;

	using Entities = std::vector<Entity*>;

	class Entity
	{
	public:
		Entity(AIWorld& world, uint16_t typeId);
		~Entity();

		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;

		Entity(Entity&&) = delete;
		Entity& operator=(Entity&&) = delete;

		SAVAGE::Math::Matrix4 LocalToWorld() const;

		AIWorld& world;
		SAVAGE::Math::Vector2 position = SAVAGE::Math::Vector2::Zero;
		SAVAGE::Math::Vector2 heading = SAVAGE::Math::Vector2::YAxis;
		const int id = 0;
		float radius = 1.0f;
	};
}
