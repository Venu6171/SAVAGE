#include "Precompiled.h"
#include "TransformComponent.h"

using namespace SAVAGE;

MEMPOOL_DEFINE(TransformComponent, 1000);

void TransformComponent::DebugUI()
{
	if (mShowTransform)
	{
		Graphics::SimpleDraw::AddLine3D(mPosition, mPosition + Math::Vector3::XAxis, Graphics::Colors::Red);
		Graphics::SimpleDraw::AddLine3D(mPosition, mPosition + Math::Vector3::YAxis, Graphics::Colors::Green);
		Graphics::SimpleDraw::AddLine3D(mPosition, mPosition + Math::Vector3::ZAxis, Graphics::Colors::Blue);
	}
}