#pragma once

#include "Component.h"

namespace SAVAGE
{
	class ModelComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Model);
		MEMPOOL_DECLARE;

		ModelComponent() = default;

		void Initialize() override;
		void Terminate() override;

		void SetFileName(const char* fileName) { mFileName = fileName; }
		void SetModelName(const char* modelName) { mModelName = modelName; }
		std::string const GetModelName() { return mModelName; }

		void SetAnimFileNames(std::vector<std::string> fileName) { mAnimFileName = fileName; }
		const ModelId GetModelId() const { return mModelId; }

	private:
		std::string mFileName;
		std::string mModelName;
		std::vector<std::string> mAnimFileName;
		ModelId mModelId;
	};
}