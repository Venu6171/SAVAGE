#pragma once

#include "TypeIds.h"

#define SET_COMPONENT_ID(id)\
	static uint32_t StaticGetTypeId() {return id;}\
	uint32_t GetTypeId() const override {return id;}

namespace SAVAGE
{
	class GameObject;

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		virtual uint32_t GetTypeId() const = 0;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void DebugUI() {}

		GameObject& GetOwner() { return *mOwner; }
		const GameObject& GetOnwer() const { return *mOwner; }

	private:
		friend class GameObject;
		GameObject* mOwner = nullptr;
	};
}