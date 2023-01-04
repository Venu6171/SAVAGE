#pragma once

#include "Colors.h"

namespace SAVAGE::Graphics
{
	class Camera;
}

namespace SAVAGE::Graphics::SimpleDraw
{
	void StaticInitialize(uint32_t maxVertexCount = 30000);
	void StaticTerminate();

	void AddLine3D(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
	void AddLine2D(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);

	// TODO
	void AddAABB3D(const Math::Vector3& center, const Math::Vector3& extend, const Color& color);
	void AddSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t slices, uint32_t rings);
	void AddTriangle(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color);

	void Render(const Camera& camera);
}