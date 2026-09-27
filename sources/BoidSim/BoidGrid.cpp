#include "BoidGrid.h"

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

	for (uint32_t i = 0; i < boidPositions.size(); i++)
	{
		const int x = Math::clamp<int>((int)(boidPositions[i].x / mCellSize.x), 0, mGridSize.x - 1);
		const int y = Math::clamp<int>((int)(boidPositions[i].y / mCellSize.y), 0, mGridSize.y - 1);

		mGrid[{x,y}].emplace_back(i);
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
