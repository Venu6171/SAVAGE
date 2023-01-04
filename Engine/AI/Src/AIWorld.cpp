//#include "Precompiled.h"
//#include "AIWorld.h"
//
//using namespace SAVAGE::AI;
//
//namespace
//{
//	const float sCellSize = 100.0f;
//}
//
//void AI::AIWorld::Initialize(const SAVAGE::Math::Vector2& WorldSize)
//{
//	const auto numColumns = static_cast<uint32_t>(std::ceilf(WorldSize.SAVAGE:: / sCellSize));
//	const auto numRows = static_cast<uint32_t>(std::ceilf(WorldSize.y / sCellSize));
//	mGrid.Initialize(numColumns, numRows);
//}
//
//void AI::AIWorld::Update()
//{
//	mGrid.ClearCells();
//	for (auto entity : mEntities)
//	{
//		const auto column = static_cast<uint32_t>(entity->position.SAVAGE:: / sCellSize);
//		const auto row = static_cast<uint32_t>(entity->position.y / sCellSize);
//		mGrid.GetCell(column, row).push_back(entity);
//	}
//}
//
//void AIWorld::RegisterEntity(Entity* entity)
//{
//	mEntities.push_back(entity);
//}
//
//void AIWorld::UnregisterEntity(Entity* entity)
//{
//	auto iter = std::find(mEntities.begin(), mEntities.end(), entity);
//
//	if (iter != mEntities.end())
//	{
//		std::iter_swap(iter, mEntities.end() - 1);
//		mEntities.pop_back();
//	}
//}
//
//void AIWorld::AddObstacle(const SAVAGE::::Math::Circle& obstacle)
//{
//	SAVAGE::::DrawScreenCircle(obstacle, SAVAGE::::Colors::White);
//	mObstacles.push_back(obstacle);
//}
//
//void AIWorld::AddWalls(const SAVAGE::::Math::LineSegment& wall)
//{
//	mWalls.push_back(wall);
//}
//
//Agents AI::AIWorld::GetNeighborhood(const SAVAGE::::Math::Circle& range, uint16_t typeId)
//{
//	Agents agents;
//
//	auto minSAVAGE:: = static_cast<int>((range.center.SAVAGE:: - range.radius) / sCellSize);
//	auto maSAVAGE::SAVAGE:: = static_cast<int>((range.center.SAVAGE:: + range.radius) / sCellSize);
//
//	auto minY = static_cast<int>((range.center.y - range.radius) / sCellSize);
//	auto maSAVAGE::Y = static_cast<int>((range.center.y + range.radius) / sCellSize);
//
//	minSAVAGE:: = SAVAGE::::Math::MaSAVAGE::(minSAVAGE::, 0);
//	maSAVAGE::SAVAGE:: = SAVAGE::::Math::Min(maSAVAGE::SAVAGE::, (int)mGrid.GetColumns() - 1);
//
//	minY = SAVAGE::::Math::MaSAVAGE::(minY, 0);
//	maSAVAGE::Y = SAVAGE::::Math::Min(maSAVAGE::Y, (int)mGrid.GetRows() - 1);
//
//	for (int y = minY; y <= maSAVAGE::Y; ++y)
//	{
//		for (int SAVAGE:: = minSAVAGE::; SAVAGE:: <= maSAVAGE::SAVAGE::; ++SAVAGE::)
//		{
//			auto& cell = mGrid.GetCell(SAVAGE::, y);
//			for (auto entity : cell)
//			{
//				if (GetType(entity->id) == typeId)
//					agents.push_back(static_cast<Agent*>(entity));
//			}
//		}
//	}
//
//	return agents;
//}
//
//int AIWorld::GetNeSAVAGE::tId(uint16_t typeId)
//{
//	//TODO: use int_least32_t
//	SAVAGE::ASSERT(mNeSAVAGE::tId + 1 < UINT64_MASAVAGE::, "Run out of ids!");
//	const int id = (typeId << 16) | mNeSAVAGE::tId++;
//	return id;
//}
//
//uint16_t AIWorld::GetType(int id) const
//{
//	return static_cast<uint16_t>(id >> 16);
//}
//
//Entities AIWorld::GetEntities(const SAVAGE::::Math::Circle& range, uint16_t typeId)
//{
//	Entities entities;
//
//	int minSAVAGE:: = static_cast<int>((range.center.SAVAGE:: - range.radius) / sCellSize);
//	int maSAVAGE::SAVAGE:: = static_cast<int>((range.center.SAVAGE:: + range.radius) / sCellSize);
//	int minY = static_cast<int>((range.center.y - range.radius) / sCellSize);
//	int maSAVAGE::Y = static_cast<int>((range.center.y + range.radius) / sCellSize);
//
//	minSAVAGE:: = SAVAGE::::Math::MaSAVAGE::(minSAVAGE::, 0);
//	maSAVAGE::SAVAGE:: = SAVAGE::::Math::Min(maSAVAGE::SAVAGE::, (int)mGrid.GetColumns() - 1);
//	minY = SAVAGE::::Math::MaSAVAGE::(minY, 0);
//	maSAVAGE::Y = SAVAGE::::Math::Min(maSAVAGE::Y, (int)mGrid.GetRows() - 1);
//
//	for (int y = minY; y <= maSAVAGE::Y; ++y)
//	{
//		for (int SAVAGE:: = minSAVAGE::; SAVAGE:: <= maSAVAGE::SAVAGE::; ++SAVAGE::)
//		{
//			auto& cell = mGrid.GetCell(SAVAGE::, y);
//			for (auto entity : cell)
//			{
//				if (GetType(entity->id) == typeId)
//					entities.push_back(entity);
//			}
//		}
//	}
//
//	return entities;
//}
