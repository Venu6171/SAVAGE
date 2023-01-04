#include "Precompiled.h"
#include "GameObjectIO.h"
#include "GameObject.h"

#include "AnimatorComponent.h"
#include "CharacterControllerComponent.h"
#include "ColliderComponent.h"
#include "EnemyComponent.h"
#include "HealthComponent.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

#include <rapidjson\document.h>
#include <rapidjson\filereadstream.h>
#include <rapidjson\filewritestream.h>

using namespace SAVAGE;
using namespace rapidjson;

void GameObjectIO::Write(FILE* file, const GameObject& gameObject)
{
	// TODO
}

void GameObjectIO::Read(FILE* file, GameObject& gameObject)
{
	// TODO: Read a file to get the component
	char buffer[65000];
	FileReadStream readStream(file, buffer, sizeof(buffer));

	Document document;
	document.ParseStream(readStream);

	auto jsonComponents = document["Components"].GetObj();
	for (auto& component : jsonComponents)
	{
		const char* componentName = component.name.GetString();
		if (strcmp(componentName, "TransformComponent") == 0)
		{
			auto transform = gameObject.AddComponent<TransformComponent>();
			if (component.value.HasMember("Position"))
			{
				const auto& position = component.value["Position"].GetArray();
				float x = position[0].GetFloat();
				float y = position[1].GetFloat();
				float z = position[2].GetFloat();
				transform->SetPosition({ x, y, z });
			}
			if (component.value.HasMember("Rotation"))
			{
				const auto& position = component.value["Rotation"].GetArray();
				float w = position[0].GetFloat();
				float x = position[1].GetFloat();
				float y = position[2].GetFloat();
				float z = position[2].GetFloat();
				transform->SetRotation({ w, x, y, z });
			}
			if (component.value.HasMember("Scale"))
			{
				const auto& position = component.value["Scale"].GetArray();
				float x = position[0].GetFloat();
				float y = position[1].GetFloat();
				float z = position[2].GetFloat();
				transform->SetScale({ x, y, z });
			}
		}
		else if (strcmp(componentName, "ColliderComponent") == 0)
		{
			auto collider = gameObject.AddComponent<ColliderComponent>();
			if (component.value.HasMember("Center"))
			{
				const auto& center = component.value["Center"].GetArray();
				float x = center[0].GetFloat();
				float y = center[1].GetFloat();
				float z = center[2].GetFloat();

				collider->SetCenter({ x, y, z });
			}
			if (component.value.HasMember("Extend"))
			{
				const auto& extend = component.value["Extend"].GetArray();
				float x = extend[0].GetFloat();
				float y = extend[1].GetFloat();
				float z = extend[2].GetFloat();

				collider->SetExtend({ x, y, z });
			}
		}
		else if (strcmp(componentName, "ModelComponent") == 0)
		{
			auto model = gameObject.AddComponent<ModelComponent>();
			if (component.value.HasMember("FileName"))
			{
				model->SetFileName(component.value["FileName"].GetString());
			}
			if (component.value.HasMember("ModelName"))
			{
				model->SetModelName(component.value["ModelName"].GetString());
			}
			if (component.value.HasMember("Animations"))
			{
				const auto& animation = component.value["Animations"].GetArray();
				std::vector<std::string> animFiles;
				animFiles.resize(animation.Size());
				for (int i = 0; i < animation.Size(); i++)
				{
					animFiles[i] = animation[i].GetString();
				}

				model->SetAnimFileNames(animFiles);
			}
		}
		else if (strcmp(componentName, "AnimatorComponent") == 0)
		{
			gameObject.AddComponent<AnimatorComponent>();
		}
		else if (strcmp(componentName, "ControllerComponent") == 0)
		{
			gameObject.AddComponent<CharacterControllerComponent>();
		}
		else if (strcmp(componentName, "EnemyComponent") == 0)
		{
			gameObject.AddComponent<EnemyComponent>();
		}
		else if (strcmp(componentName, "HealthComponent") == 0)
		{
			auto health = gameObject.AddComponent<HealthComponent>();
			if (component.value.HasMember("Health"))
			{
				health->SetHealth(component.value["Health"].GetFloat());
			}
		}
	}
}
