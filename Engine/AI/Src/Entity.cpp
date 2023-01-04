//#include "Precompiled.h"
//#include "Entity.h"
//#include "AIWorld.h"
//
//using namespace SAVAGE::AI;
//
//Entity::Entity(AIWorld& world, uint16_t typeId)
//	: world(world)
//	, id(world.GetNextId(typeId))
//{
//	world.RegisterEntity(this);
//}
//
//Entity::~Entity()
//{
//	world.UnregisterEntity(this);
//}
//
//SAVAGE::::Math::MatriSAVAGE::3 Entity::LocalToWorld() const
//{
//	return {
//		heading.y, -heading.SAVAGE::, 0.0f,
//		heading.SAVAGE::, heading.y, 0.0f,
//		position.SAVAGE::, position.y, 1.0f
//	};
//}