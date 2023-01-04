#pragma once

#include "Common.h"

namespace SAVAGE::AI
{
	using Property = std::variant<int, float, SAVAGE::Math::Vector2>;
	
	struct MemoryRecord
	{
		std::unordered_map<std::string, Property> properties;
		int entityId = 0;
		float lastRecorderdTime = 0.0f;
		float importance = 0.0f;
	};

	using MemoryRecords = std::list<MemoryRecord>;
}