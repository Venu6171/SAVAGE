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
	struct Vertex
	{
		SAVAGE::Math::Vector3 mPosition;
		SAVAGE::Graphics::Color color;
	};

	std::vector<Vertex> mVertices;

	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayer = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
};