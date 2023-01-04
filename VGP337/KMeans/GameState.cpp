#include "GameState.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <ImGui\Inc\ImPlot.h>
#include <ImGui\Src\imgui_internal.h>

using namespace SAVAGE;
using namespace SAVAGE::Graphics;
using namespace SAVAGE::Input;
using namespace SAVAGE::Math;
using namespace SAVAGE::ML;

namespace
{
	struct ClusterResult
	{
		std::vector<size_t> assignments;
		std::vector<Vector3> centroids;

		std::vector<std::vector<Vector3>> centroidHistory;

	};

	ClusterResult KMeansCluster(const std::vector<Vector3>& data, size_t k, size_t maxIteration)
	{
		ASSERT(k <= data.size(), "Not Enough data for %zu clusters!", k);

		ClusterResult clusterResult;
		clusterResult.centroids.resize(k);
		clusterResult.assignments.resize(data.size());
		clusterResult.centroidHistory.resize(k);

		// Generate random starting points
		/*for (auto& centroid : clusterResult.centroids)
		{
			centroid.x = RandomFloat(0.0f, 800.0f);
			centroid.y = RandomFloat(0.0f, 600.0f);
		}*/

		// Use existing data points as the starting cluster positions
		for (size_t i = 0; i < k; ++i)
			clusterResult.centroids[i] = data[i];

		for (size_t iter = 0; iter < maxIteration; ++iter)
		{
			for (size_t i = 0; i < data.size(); ++i)
			{
				float closestDistance = FLT_MAX;
				for (size_t j = 0; j < clusterResult.centroids.size(); ++j)
				{
					float distance = DistanceSqr(clusterResult.centroids[j], data[i]);
					if (closestDistance > distance)
					{
						closestDistance = distance;
						clusterResult.assignments[i] = j;
					}
				}
			}

			std::vector<Vector3> newCentroids;
			newCentroids.resize(k);
			int count = 0;

			for (size_t i = 0; i < k; ++i)
			{
				count = 0;
				for (size_t j = 0; j < clusterResult.assignments.size(); ++j)
				{
					if (clusterResult.assignments[j] == i)
					{
						newCentroids[i] += data[j];
						count++;
					}
				}
				newCentroids[i] /= count;
			}

			//for (size_t i = 0; i < clusterResult.centroidHistory.size(); ++i)
			//	clusterResult.centroidHistory[i].push_back(clusterResult.centroids[i]);

			for (size_t i = 0; i < newCentroids.size(); ++i)
			{
				if (newCentroids[i].x == clusterResult.centroids[i].x && newCentroids[i].y == clusterResult.centroids[i].y)
					break;
				else
					clusterResult.centroids = std::move(newCentroids);
			}
		}

		return clusterResult;
	}

	ClusterResult KMeansResult;
}

void GameState::Initialize()
{
	mUnitTexture.Initialize(L"../../Assets/Images/scv_09.png");

	for (size_t i = 0; i < 200; ++i)
	{
		float posX = RandomFloat(0.0f, 700.0f);
		float posY = RandomFloat(0.0f, 500.0f);
		float velX = RandomFloat(-100.0f, 100.0f);
		float velY = RandomFloat(-100.0f, 100.0f);
		mUnits.emplace_back(Unit{ {posX, posY }, {velX, velY} });
	}
}

void GameState::Terminate()
{
	mUnitTexture.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		SAVAGE::MainApp().Quit();

	if (inputSystem->IsKeyPressed(KeyCode::SPACE))
	{
		mShowCluster = true;
		std::vector<Vector3> data;
		data.reserve(mUnits.size());

		for (auto& unit : mUnits)
			data.emplace_back(unit.position.x, unit.position.y, 0);

		KMeansResult = KMeansCluster(data, 3, 100);
	}

	if (inputSystem->IsKeyDown(KeyCode::ENTER))
	{
		mShowCluster = false;
		for (auto& unit : mUnits)
		{
			unit.position += unit.velocity * deltaTime;

			if (unit.position.x < 0.0f)
				unit.position.x += 800.0f;
			if (unit.position.x > 800.0f)
				unit.position.x -= 800.0f;

			if (unit.position.y < 0.0f)
				unit.position.y += 600.0f;
			if (unit.position.y > 600.0f)
				unit.position.y -= 600.0f;
		}
	}
}

void GameState::DebugUI()
{
	ImGui::SetNextWindowSize({ 800.0f, 600.0f });
	ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoResize);

	const ImVec2 unitSize = ImVec2{ 32.0f, 32.0f };
	ImVec2 winPos = ImGui::GetWindowPos();
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	const ImU32 clusterColor[] = { ImColor(255, 0, 0), ImColor(0, 255, 0), ImColor(0, 0, 255), ImColor(255, 0, 255), ImColor(0, 255, 255), ImColor(255, 255, 0), ImColor(216, 222, 214) };

	for (auto& unit : mUnits)
	{
		ImGui::SetCursorPos(unit.position - (unitSize * 0.5f));
		ImGui::Image(mUnitTexture.GetRawData(), unitSize);
		drawList->AddCircle(winPos + unit.position, 10.0f, clusterColor[unit.cluster]);
	}

	if (mShowCluster)
	{
		for (size_t i = 0; i < KMeansResult.centroids.size(); ++i)
		{
			ImVec2 position;
			position.x = KMeansResult.centroids[i].x;
			position.y = KMeansResult.centroids[i].y;

			drawList->AddCircleFilled(winPos + position, 50.0f, clusterColor[i]);
		}

		for (size_t i = 0; i < KMeansResult.assignments.size(); ++i)
		{
			mUnits[i].cluster = KMeansResult.assignments[i];
			Vector3 centroid = KMeansResult.centroids[mUnits[i].cluster];
			ImVec2 center = { centroid.x, centroid.y };
			drawList->AddLine(winPos + center, winPos + mUnits[i].position, clusterColor[mUnits[i].cluster]);
		}
	}
	ImGui::End();
}