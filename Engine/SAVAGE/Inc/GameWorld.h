#pragma once

#include "GameObject.h"
#include "Service.h"
namespace SAVAGE
{
	class GameWorld final
	{
	public:
		void Initialize(uint32_t capacity);
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		template <class ServiceType>
		ServiceType* AddService()
		{
			ASSERT(!mInitialized, "GameObject - can only add service before world is initialized.");
			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());
		}

		template <class ServiceType>
		ServiceType* GetService()
		{
			for (auto& service : mServices)
			{
				if (service->GetTypeId() == ServiceType::StaticGetTypeId())
					return static_cast<ServiceType*>(service.get());
			}
			return nullptr;
		}

		GameObject* CreateGameObject(const std::filesystem::path& templateFileName, std::string name);
		GameObject* FindGameObject(const std::string& name);
		GameObject* FindGameObjectWithTag(const std::string& tag);
		std::vector<GameObject*> FindGameObjectsWithTag(const std::string& tag);
		GameObject* GetGameObject(GameObjectHandle handle);
		void DestroyGameObject(GameObjectHandle handle);

	private:
		bool IsValid(GameObjectHandle handle) const;
		void ProcessDestroyList();

		struct Slot
		{
			std::unique_ptr<GameObject> gameObject;
			uint32_t generation = 0;
		};
		
		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectSlots = std::vector<Slot>;
		using GameObjectList = std::vector<GameObject*>;

		Services mServices;

		GameObjectSlots mGameObjects;
		std::vector<uint32_t> mFreeSlots;
		std::vector<uint32_t> mToBeDestroyed;

		GameObjectList mUpdateList;
		
		bool mInitialized = false;
		bool mUpdating = false;
	};
}