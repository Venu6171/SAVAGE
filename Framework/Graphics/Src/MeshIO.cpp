#include "Precompiled.h"
#include <fstream>
#include "MeshIO.h"

using namespace SAVAGE::Graphics;

void MeshIO::Write(FILE* file, const SkinnedMesh& mesh)
{
	if (file == nullptr)
	{
		printf("Error saving a file");
		return;
	}

	const uint32_t verticesCount = static_cast<uint32_t>(mesh.vertices.size());
	fprintf_s(file, "VerticesCount: %d\n", verticesCount);
	for (uint32_t i = 0; i < verticesCount; ++i)
	{
		fprintf_s(file, "Positions: %f %f %f\n", mesh.vertices[i].position.x, mesh.vertices[i].position.y, mesh.vertices[i].position.z);
		fprintf_s(file, "Normals: %f %f %f\n", mesh.vertices[i].normal.x, mesh.vertices[i].normal.y, mesh.vertices[i].normal.z);
		fprintf_s(file, "Tangents: %f %f %f\n", mesh.vertices[i].tangent.x, mesh.vertices[i].tangent.y, mesh.vertices[i].tangent.z);
		fprintf_s(file, "UV: %f %f\n", mesh.vertices[i].uv.x, mesh.vertices[i].uv.y);
		for (uint32_t w = 0; w < std::size(mesh.vertices[i].boneWeights); ++w)
		{
			fprintf_s(file, "BoneIndices: %d\n", mesh.vertices[i].boneIndices[w]);
			fprintf_s(file, "BoneWeights: %f\n", mesh.vertices[i].boneWeights[w]);
		}
	}

	const uint32_t indicesCount = static_cast<uint32_t>(mesh.indices.size());
	fprintf_s(file, "IndicesCount: %d\n", indicesCount);
	for (uint32_t i = 0; i < indicesCount; ++i)
	{
		fprintf_s(file, "Indices: %d\n", mesh.indices[i]);
	}
}

void MeshIO::Read(FILE* file, SkinnedMesh& mesh)
{
	if (file == nullptr)
	{
		printf("Error reading a file");
		return;
	}

	uint32_t verticesCount = 0;
	char buffer[256] = { "\0" };
	fscanf_s(file, "%s %d", buffer, (int)std::size(buffer), &verticesCount);
	mesh.vertices.resize(verticesCount);
	for (uint32_t i = 0; i < verticesCount; ++i)
	{
		fscanf_s(file, "%s %f %f %f\n", buffer, (int)std::size(buffer),
			&mesh.vertices[i].position.x,
			&mesh.vertices[i].position.y,
			&mesh.vertices[i].position.z);

		fscanf_s(file, "%s %f %f %f\n", buffer, (int)std::size(buffer),
			&mesh.vertices[i].normal.x,
			&mesh.vertices[i].normal.y,
			&mesh.vertices[i].normal.z);

		fscanf_s(file, "%s %f %f %f\n", buffer, (int)std::size(buffer),
			&mesh.vertices[i].tangent.x,
			&mesh.vertices[i].tangent.y,
			&mesh.vertices[i].tangent.z);

		fscanf_s(file, "%s %f %f\n", buffer, (int)std::size(buffer),
			&mesh.vertices[i].uv.x,
			&mesh.vertices[i].uv.y);

		for (uint32_t w = 0; w < std::size(mesh.vertices[i].boneWeights); ++w)
		{
			fscanf_s(file, "%s %d\n", buffer, (int)std::size(buffer), &mesh.vertices[i].boneIndices[w]);
			fscanf_s(file, "%s %f\n", buffer, (int)std::size(buffer), &mesh.vertices[i].boneWeights[w]);
		}
	}

	uint32_t indicesCount = 0;
	buffer[0] = '\0';
	fscanf_s(file, "%s %d", buffer, (int)std::size(buffer), &indicesCount);
	mesh.indices.resize(indicesCount);

	for (uint32_t i = 0; i < indicesCount; ++i)
	{
		fscanf_s(file, "%s %d", buffer, (int)std::size(buffer), &mesh.indices[i]);
	}
}

void SAVAGE::Graphics::MeshIO::WriteBinary(FILE* file, const SkinnedMesh& mesh)
{
	if (file == nullptr)
	{
		printf("Error saving a file");
		return;
	}

	const uint32_t verticesCount = static_cast<uint32_t>(mesh.vertices.size());
	const uint32_t indicesCount = static_cast<uint32_t>(mesh.indices.size());

	fwrite(mesh.vertices.data(), verticesCount, sizeof(BoneVertex), file);
	fwrite(mesh.indices.data(), indicesCount, sizeof(uint32_t), file);
}

void SAVAGE::Graphics::MeshIO::ReadBinary(FILE* file, SkinnedMesh& mesh)
{
	if (file == nullptr)
	{
		printf("Error reading a file");
	}

	uint32_t verticesCount = 0;
	mesh.vertices.resize(verticesCount);

	uint32_t indicesCount = 0;
	mesh.indices.resize(indicesCount);

	fread(mesh.vertices.data(), verticesCount, sizeof(BoneVertex), file);
	fread(mesh.indices.data(), indicesCount, sizeof(uint32_t), file);
}
