#include "BoidGrid.h"

#include <algorithm>
#include <string>

BoidGrid::BoidGrid(const Vec2I& gridSize) :
	mGrid(gridSize), mGridSize(gridSize)
{
	const Vec2F screenBounds(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()));
	mCellSize = screenBounds / gridSize.to<float>();
}

void BoidGrid::resize(const Vec2I& gridSize)
{
	const Vec2F screenBounds(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()));
	mCellSize = screenBounds / gridSize.to<float>();
	
	mGridSize = gridSize;
	mGrid.resize(gridSize);
}

void BoidGrid::updateGrid(const std::vector<Vec2F>& boidPositions)
{
	mGrid.clear();
	mMaxDensity = 0;

	for (uint32_t i = 0; i < boidPositions.size(); i++)
	{
		const int x = Math::clamp<int>((int)(boidPositions[i].x / mCellSize.x), 0, mGridSize.x - 1);
		const int y = Math::clamp<int>((int)(boidPositions[i].y / mCellSize.y), 0, mGridSize.y - 1);

		mGrid[{x,y}].emplace_back(i);
		mMaxDensity = std::max<uint32_t>(mMaxDensity, mGrid[{ x, y }].size());
	}
}

const std::vector<int>& BoidGrid::getNeighbors(const Vec2I& gridPosition) const
{
	return mGrid[gridPosition];
}

Vec2I BoidGrid::getGridPos(const Vec2F& position) const
{
	const Vec2F gridSize = getCellSize();

	int x = Math::clamp((int)(position.x / gridSize.x), 0, mGridSize.x - 1);
	int y = Math::clamp((int)(position.y / gridSize.y), 0, mGridSize.y - 1);

	return { x, y };
}

void BoidGrid::draw() const
{
	Rectangle cell = { 0, 0, mCellSize.x, mCellSize.y };
	
	for (uint32_t i = 0; i < mGrid.getItSize(); i++)
	{
		cell.x = (float)mGrid.indexToVec2(i).x * mCellSize.x;
		cell.y = (float)mGrid.indexToVec2(i).y * mCellSize.y;

		if (const float density = (float)mGrid[i].size() / (float)mMaxDensity; !Math::nearlyEqual(density, 0.0f))
		{
			const Color color = {255, 10, 10, (uint8_t)(density * 64)};
			DrawRectangleRec(cell, color);
		}
	}
}
