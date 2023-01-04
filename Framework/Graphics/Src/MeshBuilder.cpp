#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace SAVAGE::Graphics;
using namespace SAVAGE::Math;

MeshPC MeshBuilder::CreateCubePC()
{
	MeshPC mesh;

	mesh.vertices.push_back({ Vector3{ -0.5f, 0.5f, -0.5f }, Colors::Red });
	mesh.vertices.push_back({ Vector3{ 0.5f, 0.5f, -0.5f },  Colors::Orange });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, -0.5f },Colors::Yellow });
	mesh.vertices.push_back({ Vector3{ 0.5f, -0.5f, -0.5f }, Colors::Green });
	mesh.vertices.push_back({ Vector3{ -0.5f, 0.5f, 0.5f },  Colors::Blue });
	mesh.vertices.push_back({ Vector3{ 0.5f, 0.5f, 0.5f },   Colors::Indigo });
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.5f }, Colors::Violet });
	mesh.vertices.push_back({ Vector3{ 0.5f, -0.5f, 0.5f },  Colors::White });

	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);

	mesh.indices.push_back(1);
	mesh.indices.push_back(3);
	mesh.indices.push_back(2);

	mesh.indices.push_back(4);
	mesh.indices.push_back(1);
	mesh.indices.push_back(0);

	mesh.indices.push_back(5);
	mesh.indices.push_back(1);
	mesh.indices.push_back(4);

	mesh.indices.push_back(2);
	mesh.indices.push_back(3);
	mesh.indices.push_back(6);

	mesh.indices.push_back(3);
	mesh.indices.push_back(7);
	mesh.indices.push_back(6);

	mesh.indices.push_back(0);
	mesh.indices.push_back(2);
	mesh.indices.push_back(6);

	mesh.indices.push_back(4);
	mesh.indices.push_back(0);
	mesh.indices.push_back(6);

	mesh.indices.push_back(1);
	mesh.indices.push_back(7);
	mesh.indices.push_back(3);

	mesh.indices.push_back(5);
	mesh.indices.push_back(7);
	mesh.indices.push_back(1);

	mesh.indices.push_back(5);
	mesh.indices.push_back(6);
	mesh.indices.push_back(7);

	mesh.indices.push_back(4);
	mesh.indices.push_back(6);
	mesh.indices.push_back(5);

	return mesh;
}

MeshPX MeshBuilder::CreateCubePX()
{
	MeshPX mesh;

	//LeftBottom
	//LeftTop
	//RightTop
	//RightBottom

	// Front Face
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, -0.5f, -0.5f }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, 0.5f, -0.5f }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, 0.5f, -0.5f }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, -0.5f, -0.5f }, Vector2{1.0f, 1.0f} });

	// Back Face
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, -0.5f, 0.5f }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, 0.5f, 0.5f }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, 0.5f, 0.5f }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, -0.5f, 0.5f }, Vector2{1.0f, 1.0f} });

	//// Right Face
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, -0.5f, -0.5f }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, 0.5f, -0.5f }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, 0.5f, 0.5f }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, -0.5f, 0.5f }, Vector2{1.0f, 1.0f} });

	//// Left Face
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, -0.5f, 0.5f }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, 0.5f, 0.5f }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, 0.5f, -0.5f }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, -0.5f, -0.5f }, Vector2{1.0f, 1.0f} });

	//// Upper Face
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, 0.5f, -0.5f }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, 0.5f, 0.5f }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, 0.5f, 0.5f }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, 0.5f, -0.5f }, Vector2{1.0f, 1.0f} });

	//// Bottom Face
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, -0.5f, 0.5f }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -0.5f, -0.5f, -0.5f }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, -0.5f, -0.5f }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ 0.5f, -0.5f, 0.5f }, Vector2{1.0f, 1.0f} });

	//Front Face
	mesh.indices.push_back(2);
	mesh.indices.push_back(3);
	mesh.indices.push_back(1);
	mesh.indices.push_back(1);
	mesh.indices.push_back(3);
	mesh.indices.push_back(0);

	////Back Face
	mesh.indices.push_back(6);
	mesh.indices.push_back(7);
	mesh.indices.push_back(5);
	mesh.indices.push_back(5);
	mesh.indices.push_back(7);
	mesh.indices.push_back(4);

	////Right Face
	mesh.indices.push_back(10);
	mesh.indices.push_back(11);
	mesh.indices.push_back(9);
	mesh.indices.push_back(9);
	mesh.indices.push_back(11);
	mesh.indices.push_back(8);

	////Left Face
	mesh.indices.push_back(14);
	mesh.indices.push_back(15);
	mesh.indices.push_back(13);
	mesh.indices.push_back(13);
	mesh.indices.push_back(15);
	mesh.indices.push_back(12);

	//Upper Face
	mesh.indices.push_back(18);
	mesh.indices.push_back(19);
	mesh.indices.push_back(17);
	mesh.indices.push_back(17);
	mesh.indices.push_back(19);
	mesh.indices.push_back(16);

	////Bottom Face
	mesh.indices.push_back(22);
	mesh.indices.push_back(23);
	mesh.indices.push_back(21);
	mesh.indices.push_back(21);
	mesh.indices.push_back(23);
	mesh.indices.push_back(20);

	return mesh;
}

MeshPX MeshBuilder::CreatePlanePX(uint32_t rows, uint32_t columns, float spacing, float tiling)
{
	MeshPX planeMesh;

	float xOffset = (columns - 1) * spacing * 0.5f;
	float zOffset = (rows - 1) * spacing * 0.5f;
	const float uStep = tiling / columns;
	const float vStep = tiling / (rows - 1);

	for (uint32_t r = 0; r < rows; ++r)
	{
		float v = 1.0f - (r * vStep);

		for (uint32_t c = 0; c < columns; ++c)
		{
			float x = c * spacing - xOffset;
			float z = r * spacing - zOffset;
			float u = c * uStep;
			planeMesh.vertices.push_back({ Vector3(x, 0.0f, z), Vector2(u, v) });
		}
	}

	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			planeMesh.indices.push_back((c + 0) + ((r + 0) * (columns)));
			planeMesh.indices.push_back((c + 0) + ((r + 1) * (columns)));
			planeMesh.indices.push_back((c + 1) + ((r + 1) * (columns)));

			planeMesh.indices.push_back((c + 0) + ((r + 0) * (columns)));
			planeMesh.indices.push_back((c + 1) + ((r + 1) * (columns)));
			planeMesh.indices.push_back((c + 1) + ((r + 0) * (columns)));
		}
	}

	return planeMesh;
}

MeshPX MeshBuilder::CreateCylinderPX(uint32_t rings, uint32_t slices, float height, float radius)
{
	MeshPX cylinderMesh;

	const float ringStep = height / (rings - 1);
	float sliceStep = Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings - 1);

	for (uint32_t r = 0; r < rings; ++r)
	{
		const float y = r * ringStep;
		const float v = 1.0f - (r * vStep);

		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float x = -sin(s * sliceStep) * radius;
			const float z = cos(s * sliceStep) * radius;
			const float u = s * uStep;
			cylinderMesh.vertices.push_back(VertexPX{ Vector3{x, y, z}, Vector2{u, v} });
		}
	}

	for (uint32_t r = 0; r + 1 < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			cylinderMesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			cylinderMesh.indices.push_back((s + 0) + ((r + 1) * (slices + 1)));
			cylinderMesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));

			cylinderMesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			cylinderMesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));
			cylinderMesh.indices.push_back((s + 1) + ((r + 0) * (slices + 1)));
		}
	}

	return cylinderMesh;
}

MeshPX MeshBuilder::CreateSpherePX(uint32_t rings, uint32_t slices, float radius)
{
	MeshPX sphereMesh;

	const float ringStep = Constants::Pi / (rings - 1);
	float sliceStep = Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings - 1);

	for (uint32_t r = 0; r < rings; ++r)
	{
		const float y = -cos(r * ringStep);
		const float v = 1.0f - (r * vStep);

		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float ringRadius = radius * sin(r * ringStep);
			const float x = -sin(s * sliceStep) * ringRadius;
			const float z = cos(s * sliceStep) * ringRadius;
			const float u = s * uStep;

			sphereMesh.vertices.push_back(VertexPX{ Vector3{x, y, z}, Vector2{u, v} });
		}
	}

	for (uint32_t r = 0; r + 1 < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			sphereMesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			sphereMesh.indices.push_back((s + 0) + ((r + 1) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));

			sphereMesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 0) * (slices + 1)));
		}
	}

	return sphereMesh;
}

MeshPX SAVAGE::Graphics::MeshBuilder::CreateSkySpherePX(uint32_t rings, uint32_t slices, float radius)
{
	MeshPX skySphereMesh;

	const uint32_t numVertices = ((rings + 1) * (slices + 1));
	const uint32_t numIndices = rings * (6 * (slices));

	skySphereMesh.vertices.resize(numVertices);
	skySphereMesh.indices.resize(numIndices);

	const float kSliceStep = Constants::TwoPi / (rings - 1);
	const float kRingStep = Constants::Pi / (slices - 1);
	// Fill vertex data
	float uStep = 1.0f / (slices - 1);
	float vStep = 1.0f / rings;
	uint32_t index = 0;

	for (uint32_t j = 0; j <= slices; ++j)
	{
		const float phi = j * kRingStep;
		for (uint32_t i = 0; i <= rings; ++i)
		{
			float   theta = i * kSliceStep;
			float   y = cos(phi);
			float   r = sqrt(1.0f - (y * y));
			float   s = sin(theta);
			float   c = cos(theta);
			float   x = r * c;
			float	z = r * s;
			Vector3 p(x, y, z);
			Vector2 uv((i * uStep), (j * vStep));

			if (i == rings)
			{
				index = (j * rings) + i;
				skySphereMesh.vertices[index] = skySphereMesh.vertices[index - slices];
				continue;
			}
			index = (j * rings) + i;
			skySphereMesh.vertices[index] = VertexPX({ p, uv });

		}
	}

	uint32_t indicesIndex = 0;
	// Fill index data
	for (uint32_t j = 0; j < slices - 1; ++j)
	{
		for (uint32_t i = 0; i <= rings; ++i)
		{
			const uint32_t a = i % (rings + 1);
			const uint32_t b = (i + 1) % (rings + 1);
			const uint32_t c = j * (rings + 1);
			const uint32_t d = (j + 1) * (rings + 1);

			//When i==0 && j==0
			//TRIANGLE 1
			skySphereMesh.indices[indicesIndex] = a + d; //4
			indicesIndex++;
			skySphereMesh.indices[indicesIndex] = b + c;
			indicesIndex++;						 //1
			skySphereMesh.indices[indicesIndex] = a + c;
			indicesIndex++;						 //0

			//When i==0 && j==0
			//TRIANGLE 2
			skySphereMesh.indices[indicesIndex] = a + d; //4
			indicesIndex++;
			skySphereMesh.indices[indicesIndex] = b + d;
			indicesIndex++;						 //5
			skySphereMesh.indices[indicesIndex] = b + c;
			indicesIndex++;						 //1
		}
	}

	return skySphereMesh;
}

MeshPX MeshBuilder::CreateSkyBox(float size)
{
	MeshPX mesh;

	//LeftBottom
	//LeftTop
	//RightTop
	//RightBottom

	// Front Face
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, -size, -size }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, size, -size }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ size, size, -size }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ size, -size, -size }, Vector2{1.0f, 1.0f} });

	// Back Face
	mesh.vertices.push_back(VertexPX{ Vector3{ size, -size, size }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ size, size, size }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, size, size }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, -size, size }, Vector2{1.0f, 1.0f} });

	//// Right Face
	mesh.vertices.push_back(VertexPX{ Vector3{ size, -size, -size }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ size, size, -size }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ size, size, size }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ size, -size, size }, Vector2{1.0f, 1.0f} });

	//// Left Face
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, -size, size }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, size, size }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, size, -size }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, -size, -size }, Vector2{1.0f, 1.0f} });

	//// Upper Face
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, size, -size }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, size, size }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ size, size, size }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ size, size, -size }, Vector2{1.0f, 1.0f} });

	//// Bottom Face
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, -size, size }, Vector2{0.0f, 1.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ -size, -size, -size }, Vector2{0.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ size, -size, -size }, Vector2{1.0f, 0.0f} });
	mesh.vertices.push_back(VertexPX{ Vector3{ size, -size, size }, Vector2{1.0f, 1.0f} });

	//Front Face
	mesh.indices.push_back(1);
	mesh.indices.push_back(0);
	mesh.indices.push_back(2);
	mesh.indices.push_back(2);
	mesh.indices.push_back(0);
	mesh.indices.push_back(3);

	////Back Face
	mesh.indices.push_back(5);
	mesh.indices.push_back(4);
	mesh.indices.push_back(6);
	mesh.indices.push_back(6);
	mesh.indices.push_back(4);
	mesh.indices.push_back(7);

	////Right Face
	mesh.indices.push_back(9);
	mesh.indices.push_back(8);
	mesh.indices.push_back(10);
	mesh.indices.push_back(10);
	mesh.indices.push_back(8);
	mesh.indices.push_back(11);

	////Left Face
	mesh.indices.push_back(13);
	mesh.indices.push_back(12);
	mesh.indices.push_back(14);
	mesh.indices.push_back(14);
	mesh.indices.push_back(12);
	mesh.indices.push_back(15);

	//Upper Face
	mesh.indices.push_back(17);
	mesh.indices.push_back(16);
	mesh.indices.push_back(18);
	mesh.indices.push_back(18);
	mesh.indices.push_back(16);
	mesh.indices.push_back(19);

	////Bottom Face
	mesh.indices.push_back(21);
	mesh.indices.push_back(20);
	mesh.indices.push_back(22);
	mesh.indices.push_back(22);
	mesh.indices.push_back(20);
	mesh.indices.push_back(23);

	return mesh;
}

MeshPX MeshBuilder::CreateScreenMesh()
{
	MeshPX screenMesh;

	screenMesh.vertices.push_back({ { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } });
	screenMesh.vertices.push_back({ { -1.0f, +1.0f, 0.0f }, { 0.0f, 0.0f } });
	screenMesh.vertices.push_back({ { +1.0f, +1.0f, 0.0f }, { 1.0f, 0.0f } });
	screenMesh.vertices.push_back({ { +1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } });

	screenMesh.indices.push_back(0);
	screenMesh.indices.push_back(1);
	screenMesh.indices.push_back(2);

	screenMesh.indices.push_back(0);
	screenMesh.indices.push_back(2);
	screenMesh.indices.push_back(3);

	return screenMesh;
}

MeshPN SAVAGE::Graphics::MeshBuilder::CreateSpherePN(uint32_t rings, uint32_t slices, float radius)
{
	MeshPN sphereMesh;

	const float ringStep = Constants::Pi / (rings - 1);
	float sliceStep = Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings - 1);

	for (uint32_t r = 0; r < rings; ++r)
	{
		const float y = -cos(r * ringStep);
		const float yr = sin(r * ringStep);

		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float ringRadius = radius * sin(r * ringStep);
			const float x = -sin(s * sliceStep) * ringRadius;
			const float z = cos(s * sliceStep) * ringRadius;

			sphereMesh.vertices.push_back(VertexPN{ Vector3{x, y, z}, SAVAGE::Math::Normalize(Vector3{x, y, z }) });
		}
	}

	for (uint32_t r = 0; r + 1 < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			sphereMesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			sphereMesh.indices.push_back((s + 0) + ((r + 1) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));

			sphereMesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 0) * (slices + 1)));
		}
	}

	return sphereMesh;
}

MeshPCX SAVAGE::Graphics::MeshBuilder::CreateSpherePCX(uint32_t rings, uint32_t slices, float radius)
{
	MeshPCX sphereMesh;

	Color color;

	const float ringStep = Constants::Pi / (rings - 1);
	float sliceStep = Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings - 1);

	for (uint32_t r = 0; r < rings; ++r)
	{
		const float y = -cos(r * ringStep);
		const float v = 1.0f - (r * vStep);

		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float ringRadius = radius * sin(r * ringStep);
			const float x = -sin(s * sliceStep) * ringRadius;
			const float z = cos(s * sliceStep) * ringRadius;
			const float u = s * uStep;

			//color = Colors::Red;
			//color = Colors::Orange;
			//color = Colors::Yellow;
			//color = Colors::Green;
			//color = Colors::Blue;
			//color = Colors::Indigo;
			//color = Colors::Violet;
			//color = Colors::White;

			color = Colors::Red;

			sphereMesh.vertices.push_back(VertexPCX{ Vector3{x, y, z}, Vector4(color.a, 0.0f, 0.0f, 1.0f), Vector2{u, v} });
		}
	}

	for (uint32_t r = 0; r + 1 < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			sphereMesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			sphereMesh.indices.push_back((s + 0) + ((r + 1) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));

			sphereMesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 0) * (slices + 1)));
		}
	}

	return sphereMesh;
}

Mesh SAVAGE::Graphics::MeshBuilder::CreateSphere(uint32_t rings, uint32_t slices, float radius)
{
	Mesh sphereMesh;

	const float ringStep = Constants::Pi / (rings - 1);
	float sliceStep = Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings - 1);

	for (uint32_t r = 0; r < rings; ++r)
	{
		const float y = -cos(r * ringStep);
		const float v = 1.0f - (r * vStep);

		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float ringRadius = radius * sin(r * ringStep);
			const float x = -sin(s * sliceStep) * ringRadius;
			const float z = cos(s * sliceStep) * ringRadius;
			const float u = s * uStep;

			sphereMesh.vertices.push_back(Vertex{
				Vector3{x, y, z},
				Normalize(Vector3{x, y, z }),
				Normalize(Vector3{-z, 0.0f, x}),
				{u, v} });
		}
	}

	for (uint32_t r = 0; r + 1 < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			sphereMesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			sphereMesh.indices.push_back((s + 0) + ((r + 1) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));

			sphereMesh.indices.push_back((s + 0) + ((r + 0) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 1) * (slices + 1)));
			sphereMesh.indices.push_back((s + 1) + ((r + 0) * (slices + 1)));
		}
	}


	return sphereMesh;
}

Mesh  SAVAGE::Graphics::MeshBuilder::CreatePlane(uint32_t rows, uint32_t columns, float spacing, float tiling)
{
	Mesh planeMesh;

	float xOffset = (columns - 1) * spacing * 0.5f;
	float zOffset = (rows - 1) * spacing * 0.5f;
	const float uStep = tiling / columns;
	const float vStep = tiling / (rows - 1);

	for (uint32_t r = 0; r < rows; ++r)
	{
		float v = 1.0f - (r * vStep);
		for (uint32_t c = 0; c < columns; ++c)
		{
			float x = c * spacing - xOffset;
			float z = r * spacing - zOffset;
			float u = c * uStep;
			planeMesh.vertices.push_back({
				Vector3(x, 0.0f, z),
				Vector3::YAxis,
				Vector3::XAxis,
				Vector2(u,v) });
		}
	}

	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			planeMesh.indices.push_back((c + 0) + ((r + 0) * (columns)));
			planeMesh.indices.push_back((c + 0) + ((r + 1) * (columns)));
			planeMesh.indices.push_back((c + 1) + ((r + 1) * (columns)));

			planeMesh.indices.push_back((c + 0) + ((r + 0) * (columns)));
			planeMesh.indices.push_back((c + 1) + ((r + 1) * (columns)));
			planeMesh.indices.push_back((c + 1) + ((r + 0) * (columns)));
		}
	}

	return planeMesh;
}
