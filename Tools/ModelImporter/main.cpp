#include <Graphics/Inc/Graphics.h>
#include <Math/Inc/EngineMath.h>

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include <cstdio>

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Math;

using BoneIndexLookup = std::map<std::string, int>; // Used to lookup bone index by name


struct Arguments
{
	const char* inputFileName = nullptr;
	const char* outputFileName = nullptr;
	float scale = 1.0f;
	bool useBinary = false;
	bool animOnly = false;
};

std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
	// We need at least 3 arguments.
	if (argc < 3)
		return std::nullopt;

	Arguments args;
	args.inputFileName = argv[argc - 2];
	args.outputFileName = argv[argc - 1];

	for (int i = 1; i + 2 < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'b':
				args.useBinary = true;
				break;
			case 's':
				args.scale = static_cast<float>(atof(argv[i + 1]));
				break;
			}
		}
	}
	return args;
}

void PrintUsage()
{
	printf(
		"== ModelImporter Help ==\n"
		"\n"
		"Usage:\n"
		"    ModelImporter.exe [Options] <InputFile> <OutputFile>\n"
		"\n"
		"Options:\n"
		"    -b    Use Binary format file."
		"    -s    Scale applied to the model."
		"\n"
	);
}

inline Vector2 ToVector2(const aiVector3D& v)
{
	return
	{
		static_cast<float>(v.x),
		static_cast<float>(v.y)
	};
}

inline Vector3 ToVector3(const aiVector3D& v)
{
	return
	{
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z)
	};
}

inline Quaternion ToQuaternion(const aiQuaternion& q)
{
	return
	{
		static_cast<float>(q.w),
		static_cast<float>(q.x),
		static_cast<float>(q.y),
		static_cast<float>(q.z)
	};
}

inline Color ToColor(const aiColor3D& c)
{
	return
	{
		static_cast<float>(c.r),
		static_cast<float>(c.g),
		static_cast<float>(c.b),
		1.0f
	};
}

inline Matrix4 ToMatrix4(const aiMatrix4x4& m)
{
	return
	{
		static_cast<float>(m.a1),
		static_cast<float>(m.b1),
		static_cast<float>(m.c1),
		static_cast<float>(m.d1),

		static_cast<float>(m.a2),
		static_cast<float>(m.b2),
		static_cast<float>(m.c2),
		static_cast<float>(m.d2),

		static_cast<float>(m.a3),
		static_cast<float>(m.b3),
		static_cast<float>(m.c3),
		static_cast<float>(m.d3),

		static_cast<float>(m.a4),
		static_cast<float>(m.b4),
		static_cast<float>(m.c4),
		static_cast<float>(m.d4),

	};
}

std::string GetOrCreateName(aiNode& aiNode)
{
	return aiNode.mName.C_Str();
}

void ExportEmbeddedTexture(const aiTexture* texture, const Arguments& args, const std::filesystem::path& fileName)
{
	printf("Extracting embedded texture %s\n", fileName.u8string().c_str());

	std::string fullFileName = args.outputFileName;
	fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
	fullFileName += fileName.filename().u8string().c_str();

	FILE* file = nullptr;
	auto err = fopen_s(&file, fullFileName.c_str(), "wb");
	if (err != 0 || file == nullptr)
	{
		printf("Error: failed to open file %s for saving.\n", fullFileName.c_str());
		return;
	}

	size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
	ASSERT(written == texture->mWidth, "Error: Failed to extract embedded texture!");
	fclose(file);
}


std::string FindTexture(const aiScene* scene, const aiMaterial* aiMaterial, aiTextureType textureType, const Arguments& args, std::string suffix)
{
	std::filesystem::path textureName;

	const uint32_t textureCount = aiMaterial->GetTextureCount(textureType);
	if (textureCount > 0)
	{
		aiString texturePath;
		if (aiMaterial->GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
		{
			// If texture path starts with *, then the texture is embedded.
			// https://github.com/assimp/assimp/wiki/Embedded-Textures-References
			if (texturePath.C_Str()[0] == '*')
			{
				std::string fileName = args.inputFileName;
				fileName.erase(fileName.length() - 4); // remove ".fbx" extension
				fileName += suffix;
				fileName += texturePath.C_Str()[1];

				ASSERT(scene->HasTextures(), "Error: No embedded texture found!");

				int textureIndex = atoi(texturePath.C_Str() + 1);
				ASSERT(textureIndex < (int)scene->mNumTextures, "Error: Invalid texture index!");

				const aiTexture* embeddedTexture = scene->mTextures[textureIndex];
				ASSERT(embeddedTexture->mHeight == 0, "Error: Uncompressed texture found!");

				if (embeddedTexture->CheckFormat("jpg"))
					fileName += ".jpg";
				else if (embeddedTexture->CheckFormat("png"))
					fileName += ".png";
				else
					ASSERT(embeddedTexture->mHeight == 0, "Error: Unrecogized texture format!");

				ExportEmbeddedTexture(embeddedTexture, args, fileName);

				printf("Adding texture %s\n", fileName.c_str());
				textureName = fileName;
			}
			// For FBX files, embedded textures is now accessed using GetEmbeddedTexture
			// https://github.com/assimp/assimp/issues/1830
			else if (auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str()); embeddedTexture)
			{
				std::filesystem::path embeddedFilePath = texturePath.C_Str();
				std::string fileName = args.inputFileName;
				fileName.erase(fileName.length() - 4); // remove ".fbx" extension
				fileName += suffix;
				fileName += embeddedFilePath.extension().u8string();

				ExportEmbeddedTexture(embeddedTexture, args, fileName);

				printf("Adding texture %s\n", fileName.c_str());
				textureName = fileName;
			}
			else
			{
				std::filesystem::path filePath = texturePath.C_Str();
				std::string fileName = filePath.filename().u8string();
				printf("Adding texture %s\n", fileName.c_str());
				textureName = fileName;
			}
		}
	}

	return textureName.filename().u8string().c_str();
}

// Check if aiBone exists in skeleton, if so just return the index.
// Otherwise, add it to the skeleton. The aiBone must have a name!
int TryAddBone(const aiBone* aiBone, Skeleton& skeleton, BoneIndexLookup& boneIndexLookup)
{
	std::string name = aiBone->mName.C_Str();
	ASSERT(!name.empty(), "Error: aiBone has no name!");

	auto iter = boneIndexLookup.find(name);
	if (iter != boneIndexLookup.end())
		return iter->second;

	// Add a new bone in the skeleton for this
	auto& newBone = skeleton.bones.emplace_back(std::make_unique<Bone>());
	newBone->name = std::move(name);
	newBone->index = static_cast<int>(skeleton.bones.size()) - 1;
	newBone->offsetTransform = ToMatrix4(aiBone->mOffsetMatrix);

	// Cache the bone index
	boneIndexLookup.emplace(newBone->name, newBone->index);
	return newBone->index;
}


Bone* BuildSkeleton(const aiNode& sceneNode, Bone* parent, Skeleton& skeleton, BoneIndexLookup& boneIndexLookup)
{
	Bone* bone = nullptr;

	std::string boneName = sceneNode.mName.C_Str();

	auto iter = boneIndexLookup.find(boneName);
	if (iter != boneIndexLookup.end())
	{
		bone = skeleton.bones[iter->second].get();
	}
	else
	{
		bone = skeleton.bones.emplace_back(std::make_unique<Bone>()).get();
		bone->index = static_cast<int>(skeleton.bones.size()) - 1;
		bone->offsetTransform = Matrix4::Identity();

		if (boneName.empty())
		{
			bone->name = "NoName" + std::to_string(bone->index);
		}
		else
		{
			bone->name = std::move(boneName);
		}

		// Cache the bone index
		boneIndexLookup.emplace(bone->name, bone->index);
	}

	// Link to your parent
	bone->parent = parent;
	if (parent == nullptr)
	{
		bone->parentIndex = -1;
	}
	else
	{
		bone->parentIndex = parent->index;
	}
	bone->toParentTransform = ToMatrix4(sceneNode.mTransformation);

	// Recurse through your children
	bone->children.reserve(sceneNode.mNumChildren);
	for (uint32_t i = 0; i < sceneNode.mNumChildren; ++i)
	{
		Bone* child = BuildSkeleton(*(sceneNode.mChildren[i]), bone, skeleton, boneIndexLookup);
		bone->children.push_back(child);
		bone->childIndices.push_back(child->index);
	}

	return bone;
}

void SaveModel(const Arguments& args, const Model& model)
{
	if (model.meshData.empty())
	{
		return;
	}

	printf("Saving model: %s...\n", args.outputFileName);

	FILE* file = nullptr;
	auto err = fopen_s(&file, args.outputFileName, args.useBinary ? "wb" : "w");
	if (err != 0 || file == nullptr)
	{
		printf("Error: failed to open file %s for saving.\n", args.outputFileName);
		return;
	}

	const uint32_t meshCount = static_cast<uint32_t>(model.meshData.size());
	fprintf_s(file, "MeshCount: %d\n", meshCount);
	for (uint32_t i = 0; i < meshCount; ++i)
	{
		fprintf_s(file, "MaterialIndex: %d\n", model.meshData[i]->materialIndex);
		if (args.useBinary)
		{
			MeshIO::WriteBinary(file, model.meshData[i]->mesh);
		}
		else
			MeshIO::Write(file, model.meshData[i]->mesh);
	}

	fclose(file);
}

void SaveMaterial(const Arguments& args, const Model& model)
{
	if (model.materialData.empty())
		return;

	std::filesystem::path path = args.outputFileName;
	path.replace_extension("material");

	printf("Saving material: %s...\n", path.u8string().c_str());

	FILE* file = nullptr;
	auto err = fopen_s(&file, path.u8string().c_str(), "w");
	if (err != 0 || file == nullptr)
	{
		printf("Error: failed to open file %s for saving.\n", path.u8string().c_str());
		return;
	}

	const uint32_t materialCount = static_cast<uint32_t>(model.materialData.size());
	fprintf_s(file, "MaterialCount: %d\n", materialCount);
	for (const auto& materialData : model.materialData)
	{
		MaterialIO::Write(file, materialData);
		// Note: you cannot write an empty string, so if the material name is empty, you can write "none".
		// Just make sure the loading code checks for that.
	}

	fclose(file);
}

void SaveSkeleton(const Arguments& args, const Model& model)
{
	if (!model.skeleton)
		return;

	std::filesystem::path path = args.outputFileName;
	path.replace_extension("skeleton");

	printf("Saving skeleton: %s...\n", path.u8string().c_str());

	FILE* file = nullptr;
	auto err = fopen_s(&file, path.u8string().c_str(), "w");
	if (err != 0 || file == nullptr)
	{
		printf("Error: failed to open file %s for saving.\n", path.u8string().c_str());
		return;
	}

	SkeletonIO::Write(file, *(model.skeleton));

	fclose(file);
}

void SaveAnimation(const Arguments& args, const Model& model)
{
	if (model.animSet.empty())
		return;

	std::filesystem::path path = args.outputFileName;
	path.replace_extension("animset");

	printf("Saving Animation: %s...\n", path.u8string().c_str());

	FILE* file = nullptr;
	auto err = fopen_s(&file, path.u8string().c_str(), "w");
	if (err != 0 || file == nullptr)
	{
		printf("Error: failed to open file %s for saving.\n", path.u8string().c_str());
		return;
	}

	for (const auto& animData : model.animSet)
	{
		AnimationIO::Write(file, *animData);
	}

	fclose(file);
}

int main(int argc, char* argv[])
{
	const auto argsOpt = ParseArgs(argc, argv);
	if (!argsOpt.has_value())
	{
		PrintUsage();
		return -1;
	}

	const auto& args = argsOpt.value();

	// Create an instance of the importer class to do the parsing for us.
	Assimp::Importer importer;

	// Try to import the model into a scene.
	const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;
	const aiScene* scene = importer.ReadFile(args.inputFileName, flags);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}

	Model model;
	BoneIndexLookup boneIndexLookup;

	// Look for mesh data
	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");

		const uint32_t numMeshes = scene->mNumMeshes;
		model.meshData.reserve(numMeshes);

		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const auto aiMesh = scene->mMeshes[meshIndex];
			const uint32_t numVertices = aiMesh->mNumVertices;
			const uint32_t numFaces = aiMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			auto& meshData = model.meshData.emplace_back(std::make_unique<Model::MeshData>());

			printf("Reading material index...\n");

			meshData->materialIndex = aiMesh->mMaterialIndex;

			printf("Reading vertices...\n");

			auto& mesh = meshData->mesh;
			mesh.vertices.reserve(numVertices);

			const aiVector3D* positions = aiMesh->mVertices;
			const aiVector3D* normals = aiMesh->mNormals;
			const aiVector3D* tangents = aiMesh->mTangents;
			const aiVector3D* texCoords = aiMesh->HasTextureCoords(0) ? aiMesh->mTextureCoords[0] : nullptr;
			for (uint32_t i = 0; i < numVertices; ++i)
			{
				auto& vertex = mesh.vertices.emplace_back(BoneVertex{});
				vertex.position = ToVector3(positions[i]) * args.scale;
				vertex.normal = ToVector3(normals[i]);
				vertex.tangent = ToVector3(tangents[i]);
				vertex.uv = texCoords ? ToVector2(texCoords[i]) : Vector2::Zero;
			}

			printf("Reading indices...\n");

			mesh.indices.reserve(numIndices);

			const auto aiFaces = aiMesh->mFaces;
			for (uint32_t i = 0; i < numFaces; ++i)
			{
				auto& aiFace = aiFaces[i];
				mesh.indices.push_back(aiFace.mIndices[0]);
				mesh.indices.push_back(aiFace.mIndices[1]);
				mesh.indices.push_back(aiFace.mIndices[2]);
			}

			if (aiMesh->HasBones())
			{
				if (!model.skeleton)
					model.skeleton = std::make_unique<Skeleton>();

				printf("Reading bone weights...\n");

				// Track how many weights have we added to each vertex
				std::vector<int> numWeightsAdded(mesh.vertices.size(), 0);

				for (uint32_t meshBoneIndex = 0; meshBoneIndex < aiMesh->mNumBones; ++meshBoneIndex)
				{
					const auto aiBone = aiMesh->mBones[meshBoneIndex];
					const int boneIndex = TryAddBone(aiBone, *model.skeleton, boneIndexLookup);

					for (uint32_t weightIndex = 0; weightIndex < aiBone->mNumWeights; ++weightIndex)
					{
						const auto& vertexWeight = aiBone->mWeights[weightIndex];
						auto& vertex = mesh.vertices[vertexWeight.mVertexId];
						auto& added = numWeightsAdded[vertexWeight.mVertexId];
						if (added < 4)
						{
							vertex.boneIndices[added] = boneIndex;
							vertex.boneWeights[added] = vertexWeight.mWeight;
							++added;
						}
					}
				}
			}
		}
	}

	if (scene->HasMaterials())
	{
		printf("Reading Materials...\n");

		const uint32_t numMaterials = scene->mNumMaterials;
		model.materialData.resize(numMaterials);

		for (uint32_t materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
		{
			const auto aiMaterial = scene->mMaterials[materialIndex];

			aiColor3D ambientColor, diffuseColor, specularColor;
			ai_real specularPower = 1.0f;

			aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
			aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
			aiMaterial->Get(AI_MATKEY_SHININESS, specularPower);

			auto& materialData = model.materialData[materialIndex];
			materialData.material.ambient = ToColor(ambientColor);
			materialData.material.diffuse = ToColor(diffuseColor);
			materialData.material.specular = ToColor(specularColor);
			materialData.material.power = specularPower;
			materialData.diffuseMapName = FindTexture(scene, aiMaterial, aiTextureType_DIFFUSE, args, std::string("_Diffuse_") + std::to_string(materialIndex));
			materialData.specularMapName = FindTexture(scene, aiMaterial, aiTextureType_SHININESS, args, std::string("_Specular_") + std::to_string(materialIndex));
			materialData.normalMapName = FindTexture(scene, aiMaterial, aiTextureType_NORMALS, args, std::string("_Normal_") + std::to_string(materialIndex));
		}
	}

	if (model.skeleton)
	{
		printf("Building Skeleton...\n");

		BuildSkeleton(*scene->mRootNode, nullptr, *(model.skeleton), boneIndexLookup);

		for (auto& bone : model.skeleton->bones)
		{
			bone->offsetTransform._41 *= args.scale;
			bone->offsetTransform._42 *= args.scale;
			bone->offsetTransform._43 *= args.scale;

			bone->toParentTransform._41 *= args.scale;
			bone->toParentTransform._42 *= args.scale;
			bone->toParentTransform._43 *= args.scale;
		}
	}

	if (scene->HasAnimations())
	{
		printf("Reading animations...\n");

		for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
		{
			const auto aiAnimation = scene->mAnimations[animIndex];
			auto& animClip = model.animSet.emplace_back(std::make_unique<AnimationClip>());

			if (aiAnimation->mName.length > 0)
				animClip->name = aiAnimation->mName.C_Str();
			else
				animClip->name = "Anim" + std::to_string(animIndex);

			animClip->duration = static_cast<float>(aiAnimation->mDuration);
			animClip->ticksPerSecond = static_cast<float>(aiAnimation->mTicksPerSecond);

			printf("Reading bone animations for %s ...\n", animClip->name.c_str());

			// Reserve space so we have one animation slot per bone, note that not
			// all bones will have animations so some slot will stay empty. However,
			// keeping them the same size means we can use bone index directly to look
			// up animations.
			animClip->boneAnimations.resize(model.skeleton->bones.size());
			for (uint32_t boneAnimIndex = 0; boneAnimIndex < aiAnimation->mNumChannels; ++boneAnimIndex)
			{
				const aiNodeAnim* aiBoneAnim = aiAnimation->mChannels[boneAnimIndex];
				const int slotIndex = boneIndexLookup[aiBoneAnim->mNodeName.C_Str()];
				auto& boneAnim = animClip->boneAnimations[slotIndex];
				boneAnim = std::make_unique<Animation>();

				AnimationBuilder builder;
				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumPositionKeys; ++keyIndex)
				{
					auto& key = aiBoneAnim->mPositionKeys[keyIndex];
					builder.AddPositionKey(ToVector3(key.mValue) * args.scale, static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumRotationKeys; ++keyIndex)
				{
					auto& key = aiBoneAnim->mRotationKeys[keyIndex];
					builder.AddRotationKey(ToQuaternion(key.mValue), static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumScalingKeys; ++keyIndex)
				{
					auto& key = aiBoneAnim->mScalingKeys[keyIndex];
					builder.AddScaleKey(ToVector3(key.mValue), static_cast<float>(key.mTime));
				}

				*boneAnim = builder.Get();
			}
		}
	}


	// Save mesh data into .model file
	SaveModel(args, model);

	//Save material data into .material file
	SaveMaterial(args, model);

	// Save skeleton data into .skeleton file
	SaveSkeleton(args, model);

	// Save aniamtion into .animation file
	SaveAnimation(args, model);

	printf("All done!\n");

	return 0;
}