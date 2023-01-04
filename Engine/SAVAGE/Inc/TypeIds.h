#pragma once

#include "Common.h"

namespace SAVAGE
{
	enum ComponentId
	{
		Animator,
		Collider,
		Controller,
		EnemyController,
		Health,
		Model,
		TransformId
	};

	enum ServiceId
	{
		Render
	};
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() {return id;}\
	uint32_t GetTypeId() const override {return id;}
