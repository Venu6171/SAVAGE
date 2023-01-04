#include "Precompiled.h"
#include "AnimationIO.h"
#include "Material.h"
#include "MaterialIO.h"
#include "Model.h"
#include "ModelLoader.h"
#include "MeshIO.h"
#include "SkeletonIO.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;

void ModelLoader::LoadObjPX(std::filesystem::path fileName, float scale, MeshPX& mesh)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	ASSERT(file, "ModelLoader - Failed to open model %s.", fileName.u8string().c_str());

	std::vector<Math::Vector3> positions;
	std::vector<Math::Vector2> uvs;
	std::vector<uint32_t> positionIndices, uvIndices;

	while (true)
	{
		char buffer[256];
		int result = fscanf_s(file, "%s", buffer, (int)std::size(buffer));

		// If we reached the end of the file, we are done
		if (result == EOF)
			break;

		if (strcmp(buffer, "v") == 0)
		{
			Math::Vector3 position;
			fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			positions.push_back(position * scale);
		}
		else if (strcmp(buffer, "vt") == 0)
		{
			Math::Vector2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back({ uv.x, 1.0f - uv.y });
		}
		else if (strcmp(buffer, "f") == 0)
		{
			uint32_t v[4], t[4];
			fgets(buffer, (int)std::size(buffer), file);

			if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d\n",
				&v[0], &t[0],
				&v[1], &t[1],
				&v[2], &t[2]) == 6)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
			}
			else if (sscanf_s(buffer, "%d/%d %d/%d %d/%d %d/%d\n",
				&v[0], &t[0],
				&v[1], &t[1],
				&v[2], &t[2],
				&v[3], &t[3]) == 8)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				positionIndices.push_back(v[3]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				uvIndices.push_back(t[3]);
			}
		}
	}

	fclose(file);

	// Reconstruct vertices
	std::vector<VertexPX> vertices;
	vertices.reserve(positionIndices.size());
	for (size_t i = 0; i < positionIndices.size(); ++i)
	{
		VertexPX vertex;
		vertex.position = positions[positionIndices[i] - 1];
		vertex.uv = uvs[uvIndices[i] - 1];
		vertices.push_back(vertex);
	}

	// Generate indices
	std::vector<uint32_t> indices;
	indices.resize(vertices.size());
	std::iota(indices.begin(), indices.end(), 0);

	// Initialize mesh
	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);
}

void ModelLoader::LoadObj(std::filesystem::path fileName, float scale, Mesh& mesh)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	ASSERT(file, "ModelLoader - Failed to open model %s.", fileName.u8string().c_str());

	std::vector<Math::Vector3> positions;
	std::vector<Math::Vector3> normals;
	std::vector<Math::Vector2> uvs;
	std::vector<uint32_t> positionIndices, normalIndices, uvIndices;

	while (true)
	{
		char buffer[256];
		int result = fscanf_s(file, "%s", buffer, (int)std::size(buffer));

		// If we reached the end of the file, we are done
		if (result == EOF)
			break;

		if (strcmp(buffer, "v") == 0)
		{
			Math::Vector3 position;
			fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			positions.push_back(position * scale);
		}
		else if (strcmp(buffer, "vn") == 0)
		{
			Math::Vector3 normal;
			fscanf_s(file, "%f %f %f.;m\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(SAVAGE::Math::Normalize(normal * scale));
		}
		else if (strcmp(buffer, "vt") == 0)
		{
			Math::Vector2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back({ uv.x, 1.0f - uv.y });
		}
		else if (strcmp(buffer, "f") == 0)
		{
			uint32_t v[4], t[4], n[4];
			fgets(buffer, (int)std::size(buffer), file);

			if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2]) == 9)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
			}
			else if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2],
				&v[3], &t[3], &n[3]) == 12)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				positionIndices.push_back(v[3]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);
				normalIndices.push_back(n[4]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				uvIndices.push_back(t[3]);
			}
		}
	}

	fclose(file);

	// Reconstruct vertices
	std::vector<Vertex> vertices;
	vertices.reserve(positionIndices.size());
	for (size_t i = 0; i < positionIndices.size(); ++i)
	{
		Vertex vertex;
		vertex.position = positions[positionIndices[i] - 1];
		vertex.normal = SAVAGE::Math::Normalize(vertex.position);
		vertex.tangent;
		vertex.uv = uvs[uvIndices[i] - 1];
		vertices.push_back(vertex);
	}

	// Generate indices
	std::vector<uint32_t> indices;
	indices.resize(vertices.size());
	std::iota(indices.begin(), indices.end(), 0);

	// Initialize mesh
	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);
}

void ModelLoader::LoadModel(std::filesystem::path fileName, Model& model, bool binary)
{
	FILE* file = nullptr;

	fopen_s(&file, fileName.u8string().c_str(), binary ? "rb" : "r");
	ASSERT(file, "Mesh - Failed to read mesh data %s.", fileName.u8string().c_str());

	while (true)
	{
		char buffer[256] = { "\0" };
		uint32_t meshCount = 0;
		int result = fscanf_s(file, "%s %d\n", buffer, (int)std::size(buffer), &meshCount);

		// If we reached the end of the file, we are done
		if (result == EOF)
			break;
		buffer[0] = '\0';
		for (uint32_t i = 0; i < meshCount; ++i)
		{
			std::unique_ptr<Model::MeshData> meshData = std::make_unique<Model::MeshData>();
			uint32_t materialIndex = 0;
			fscanf_s(file, "%s %d\n", buffer, (int)std::size(buffer), &materialIndex);
			meshData->materialIndex = materialIndex;
			MeshIO::Read(file, meshData->mesh);
			model.meshData.emplace_back(std::move(meshData));
		}
	}
	fclose(file);
}

void ModelLoader::LoadMaterial(std::filesystem::path fileName, Model& model)
{
	FILE* file = nullptr;

	fopen_s(&file, fileName.u8string().c_str(), "r");
	ASSERT(file, "Material - Failed to read material data %s.", fileName.u8string().c_str());

	while (true)
	{
		char buffer[256] = { "\0" };
		uint32_t materialCount = 0;
		int result = fscanf_s(file, "%s %d\n", buffer, (int)std::size(buffer), &materialCount);

		// If we reached the end of the file, we are done
		if (result == EOF)
			break;

		for (uint32_t i = 0; i < materialCount; ++i)
		{
			Model::MaterialData materialData;
			MaterialIO::Read(file, materialData);
			model.materialData.emplace_back(std::move(materialData));
		}
	}
	fclose(file);
}

void ModelLoader::LoadSkeleton(std::filesystem::path fileName, Model& model)
{
	FILE* file = nullptr;

	fopen_s(&file, fileName.u8string().c_str(), "r");
	ASSERT(file, "Skeleton - Failed to read skeleton data %s.", fileName.u8string().c_str());

	model.skeleton = std::make_unique<Skeleton>();

	while (true)
	{
		char buffer[256] = { "\0" };
		uint32_t bonesCount = 0;
		int result = fscanf_s(file, "%s %d\n", buffer, (int)std::size(buffer), &bonesCount);

		// If we reached the end of the file, we are done
		if (result == EOF)
			break;

		for (uint32_t i = 0; i < bonesCount; ++i)
		{
			std::unique_ptr<Bone> boneToLoad = std::make_unique<Bone>();
			SkeletonIO::Read(file, *boneToLoad);
			model.skeleton->bones.emplace_back(std::move(boneToLoad));
		}

	}
	fclose(file);
}

void ModelLoader::LoadAnimationClip(std::filesystem::path fileName, Model& model)
{
	FILE* file = nullptr;

	fopen_s(&file, fileName.u8string().c_str(), "r");
	ASSERT(file, "Animation - Failed to read animation data %s.", fileName.u8string().c_str());

	std::unique_ptr<AnimationClip> animClip = std::make_unique<AnimationClip>();

	AnimationIO::Read(file, *animClip);

	fclose(file);

	model.animSet.emplace_back(std::move(animClip));
}