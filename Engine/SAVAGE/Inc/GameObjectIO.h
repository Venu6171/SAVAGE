#pragma once

namespace SAVAGE
{
	class GameObject;

	namespace GameObjectIO
	{
		void Write(FILE* file, const GameObject& gameObject);
		void Read(FILE* file, GameObject& gameObject);
	}
}