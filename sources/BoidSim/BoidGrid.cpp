#include "BoidGrid.h"

BoidGrid::BoidGrid()
{
	mGrid.fill(std::array<std::vector<int>, GRID_SIZE>());

	for (size_t y = 0; y < GRID_SIZE; y++)
	{
		mGrid[y].fill(std::vector<int>());
	}
}

void BoidGrid::updateGrid(const std::vector<Vec2F>& boidPositions)
{
	const Vec2F gridSize = getGridSize();

	for (size_t y = 0; y < GRID_SIZE; y++)
	{
		for (size_t x = 0; x < GRID_SIZE; x++)
		{
			mGrid[y][x].clear();
		}
	}

	for (uint32_t i = 0; i < boidPositions.size(); i++)
	{
		const size_t x = Math::clamp<int>((int)(boidPositions[i].x / gridSize.x), 0, GRID_SIZE - 1);
		const size_t y = Math::clamp<int>((int)(boidPositions[i].y / gridSize.y), 0, GRID_SIZE - 1);

		mGrid[y][x].emplace_back(i);
	}
}

const std::vector<int>& BoidGrid::getNeighbors(const Vec2I& gridPosition) const
{
	return mGrid[gridPosition.y][gridPosition.x];
}

Vec2I BoidGrid::getGridPos(const Vec2F& position) const
{
	const Vec2F gridSize = getGridSize();

	int x = Math::clamp((int)(position.x / gridSize.x), 0, GRID_SIZE - 1);
	int y = Math::clamp((int)(position.y / gridSize.y), 0, GRID_SIZE - 1);

	return { x, y };
}

Vec2F BoidGrid::getGridSize() const
{
	const Vec2F screenBounds(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()));
	return screenBounds / Vec2F(GRID_SIZE, GRID_SIZE);
}
