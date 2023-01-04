#pragma once

#include "Agent.h"
#include "PartitionGrid.h"

//namespace SAVAGE::AI
//{
//	class AIWorld
//	{
//	public:
//		using Obstacles = std::vector<SAVAGE::Math::Circle>;
//		using Walls = std::vector <SAVAGE::Math::LineSegment>;
//
//		void Initialize(const SAVAGE::Math::Vector2& WorldSize);
//		void Update();
//
//		void RegisterEntity(Entity* entity);
//		void UnregisterEntity(Entity* entity);
//
//		void AddObstacle(const SAVAGE::Math::Circle& obstacle);
//		void AddWalls(const SAVAGE::Math::LineSegment& wall);
//
//		Agents GetNeighborhood(const SAVAGE::Math::Circle& range, uint16_t typeId);
//
//		int GetNextId(uint16_t typeId);
//		uint16_t GetType(int id) const;
//
//		Entities GetEntities(const SAVAGE::Math::Circle& range, uint16_t typeId);
//
//		const Obstacles& GetObstacles() const { return mObstacles; }
//		const Walls& GetWalls() const { return mWalls; }
//	private:
//		Entities mEntities;
//		Obstacles mObstacles;
//		Walls mWalls;
//		PartitionGrid<Entity> mGrid;
//		int mNextId = 0;
//	};
//}