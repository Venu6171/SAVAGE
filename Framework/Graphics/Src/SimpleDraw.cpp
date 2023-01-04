#include "Precompiled.h"
#include "SimpleDraw.h"

#include "Camera.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"
#include "GraphicsSystem.h"

using namespace SAVAGE;
using namespace SAVAGE::Graphics;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine3D(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
		void AddLine2D(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
		void AddSphere(const Math::Vector3& center, const float radius, const Color& color, uint32_t slices, uint32_t rings);
		void Render(const Camera& camera);

		GraphicsSystem gs;

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		ConstantBuffer mConstantBuffer;

		MeshBuffer mMeshBuffer;
		std::unique_ptr<VertexPC[]> mLineVertices3D;
		std::unique_ptr<VertexPC[]> mLineVertices2D;

		uint32_t mLineVertex3DCount = 0;
		uint32_t mMaxVertex3DCount = 0;

		uint32_t mLineVertex2DCount = 0;
		uint32_t mMaxVertex2DCount = 0;
	};

	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		mVertexShader.Initialize("../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
		mPixelShader.Initialize("../../Assets/Shaders/SimpleDraw.fx");
		mConstantBuffer.Initialize(sizeof(Math::Matrix4));
		mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount, true);

		mLineVertices3D = std::make_unique<VertexPC[]>(maxVertexCount);
		mLineVertices2D = std::make_unique<VertexPC[]>(maxVertexCount);

		mLineVertex3DCount = 0;
		mLineVertex2DCount = 0;

		mMaxVertex3DCount = maxVertexCount;
		mMaxVertex2DCount = maxVertexCount;
	}

	void SimpleDrawImpl::Terminate()
	{
		mMeshBuffer.Terminate();
		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
	}

	void SimpleDrawImpl::AddLine3D(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
	{
		if (mLineVertex3DCount + 2 <= mMaxVertex3DCount)
		{
			mLineVertices3D[mLineVertex3DCount++] = VertexPC{ v0, color };
			mLineVertices3D[mLineVertex3DCount++] = VertexPC{ v1, color };
		}
	}

	void SimpleDrawImpl::AddLine2D(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
	{
		if (mLineVertex2DCount + 2 <= mMaxVertex2DCount)
		{
			mLineVertices2D[mLineVertex2DCount++] = VertexPC{ v0, color };
			mLineVertices2D[mLineVertex2DCount++] = VertexPC{ v1, color };
		}
	}

	void SimpleDrawImpl::AddSphere(const Math::Vector3& center, const float radius, const Color& color, uint32_t slices, uint32_t rings)
	{
		const float x = center.x;
		const float y = center.y;
		const float z = center.z;

		const uint32_t kSlices = Math::Max(3u, slices);
		const uint32_t kRings = Math::Max(2u, rings);
		const uint32_t kLines = (4 * kSlices * kRings) - (2 * kSlices);

		// Check if we have enough space
		if (mLineVertex3DCount + kLines <= mMaxVertex3DCount)
		{
			// Add lines
			const float kTheta = Math::Constants::Pi / (float)kRings;
			const float kPhi = Math::Constants::TwoPi / (float)kSlices;
			for (uint32_t j = 0; j < kSlices; ++j)
			{
				for (uint32_t i = 0; i < kRings; ++i)
				{
					const float a = i * kTheta;
					const float b = a + kTheta;
					const float ay = radius * cos(a);
					const float by = radius * cos(b);

					const float theta = j * kPhi;
					const float phi = theta + kPhi;

					const float ar = sqrt(radius * radius - ay * ay);
					const float br = sqrt(radius * radius - by * by);

					const float x0 = x + (ar * sin(theta));
					const float y0 = y + (ay);
					const float z0 = z + (ar * cos(theta));

					const float x1 = x + (br * sin(theta));
					const float y1 = y + (by);
					const float z1 = z + (br * cos(theta));

					const float x2 = x + (br * sin(phi));
					const float y2 = y + (by);
					const float z2 = z + (br * cos(phi));

					mLineVertices3D[mLineVertex3DCount++] = { Math::Vector3(x0, y0, z0), color };
					mLineVertices3D[mLineVertex3DCount++] = { Math::Vector3(x1, y1, z1), color };

					if (i < kRings - 1)
					{
						mLineVertices3D[mLineVertex3DCount++] = { Math::Vector3(x1, y1, z1), color };
						mLineVertices3D[mLineVertex3DCount++] = { Math::Vector3(x2, y2, z2), color };
					}
				}
			}
		}
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{
		// 3D
		auto matView = camera.GetViewMatrix();
		auto matProj = camera.GetProjectionMatrix();
		auto transform = Math::Transpose(matView * matProj);
		mConstantBuffer.Update(&transform);
		mConstantBuffer.BindVS(0);

		mVertexShader.Bind();
		mPixelShader.Bind();

		mMeshBuffer.Update(mLineVertices3D.get(), mLineVertex3DCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		mLineVertex3DCount = 0;

		// 2D
		const uint32_t w = gs.GetBackBufferWidth();
		const uint32_t h = gs.GetBackBufferHeight();
		Math::Matrix4 matInvScreen
		{
			2.0f / w, 0.0f, 0.0f, 0.0f,
			0.0f, -2.0f / h, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
		};

		transform = Math::Transpose(matInvScreen);
		mConstantBuffer.Update(&transform);
		mConstantBuffer.BindVS(0);

		mVertexShader.Bind();
		mPixelShader.Bind();

		mMeshBuffer.Update(mLineVertices2D.get(), mLineVertex2DCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		mLineVertex2DCount = 0;
	}

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SAVAGE::Graphics::SimpleDraw::AddLine3D(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
	sInstance->AddLine3D(v0, v1, color);
}

void SAVAGE::Graphics::SimpleDraw::AddLine2D(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
	sInstance->AddLine2D(v0, v1, color);
}

void SAVAGE::Graphics::SimpleDraw::AddAABB3D(const Math::Vector3& center, const Math::Vector3& extend, const Color& color)
{
	float minX = center.x - extend.x;
	float minY = center.y - extend.y;
	float minZ = center.z - extend.z;
	float maxX = center.x + extend.x;
	float maxY = center.y + extend.y;
	float maxZ = center.z + extend.z;

	// Call sInstance->AddLine 8 times here ...
	sInstance->AddLine3D({ minX, minY, minZ }, { minX, minY, maxZ }, color);
	sInstance->AddLine3D({ minX, minY, maxZ }, { maxX, minY, maxZ }, color);
	sInstance->AddLine3D({ maxX, minY, maxZ }, { maxX, minY, minZ }, color);
	sInstance->AddLine3D({ maxX, minY, minZ }, { minX, minY, minZ }, color);
	sInstance->AddLine3D({ minX, minY, minZ }, { minX, maxY, minZ }, color);
	sInstance->AddLine3D({ minX, minY, maxZ }, { minX, maxY, maxZ }, color);
	sInstance->AddLine3D({ maxX, minY, maxZ }, { maxX, maxY, maxZ }, color);
	sInstance->AddLine3D({ maxX, minY, minZ }, { maxX, maxY, minZ }, color);
	sInstance->AddLine3D({ minX, maxY, minZ }, { minX, maxY, maxZ }, color);
	sInstance->AddLine3D({ minX, maxY, maxZ }, { maxX, maxY, maxZ }, color);
	sInstance->AddLine3D({ maxX, maxY, maxZ }, { maxX, maxY, minZ }, color);
	sInstance->AddLine3D({ maxX, maxY, minZ }, { minX, maxY, minZ }, color);
}

void SAVAGE::Graphics::SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t slices, uint32_t rings)
{
	sInstance->AddSphere(center, radius, color, slices, rings);
}

void SAVAGE::Graphics::SimpleDraw::AddTriangle(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color)
{
	sInstance->AddLine3D(v0, v1, color);
	sInstance->AddLine3D(v1, v2, color);
	sInstance->AddLine3D(v2, v0, color);
}

void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}