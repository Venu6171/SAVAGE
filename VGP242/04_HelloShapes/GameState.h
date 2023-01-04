#pragma once

#include "SAVAGE/Inc/SAVAGE.h"

class GameState : public SAVAGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render();

private:
	std::vector<SAVAGE::Graphics::VertexPC> mHeartVertices;
	std::vector<SAVAGE::Graphics::VertexPC> mTriforceVertices;
	std::vector<SAVAGE::Graphics::VertexPC> mSwordVertices;

	SAVAGE::Graphics::MeshBuffer mVertexBuffer[3];
	SAVAGE::Graphics::MeshBuffer* mCurrentVertexBuffer = nullptr;

	SAVAGE::Graphics::VertexShader mVertexShader;
	SAVAGE::Graphics::PixelShader mPixelShader;
};