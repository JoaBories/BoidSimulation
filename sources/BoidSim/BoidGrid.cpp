#include "BoidGrid.h"

BoidGrid::BoidGrid()
{
	mGrid.fill(std::array<std::vector<int>, SIZE_Y>());

	for (size_t y = 0; y < SIZE_Y; y++)
	{
		mGrid[y].fill(std::vector<int>());
	}
}

void BoidGrid::updateGrid(const std::vector<Vect2F>& boidPositions)
{
	const Vect2F gridSize = getGridSize();

	for (size_t y = 0; y < SIZE_Y; y++)
	{
		for (size_t x = 0; x < SIZE_X; x++)
		{
			mGrid[y][x].clear();
		}
	}

	for (uint32_t i = 0; i < boidPositions.size(); i++)
	{
		const size_t x = Math::Clamp<int>((int)(boidPositions[i].x / gridSize.x), 0, SIZE_X - 1);
		const size_t y = Math::Clamp<int>((int)(boidPositions[i].y / gridSize.y), 0, SIZE_Y - 1);

		mGrid[y][x].emplace_back(i);
	}
}

const std::vector<int>& BoidGrid::getNeighbors(const Vect2I& gridPosition) const
{
	return mGrid[gridPosition.y][gridPosition.x];
}

Vect2I BoidGrid::getGridPos(const Vect2F& position) const
{
	const Vect2F gridSize = getGridSize();

	int x = Math::Clamp((int)(position.x / gridSize.x), 0, SIZE_X - 1);
	int y = Math::Clamp((int)(position.y / gridSize.y), 0, SIZE_Y - 1);

	return { x, y };
}

Vect2F BoidGrid::getGridSize() const
{
	const Vect2F screenBounds(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()));
	return screenBounds / Vect2F(SIZE_X, SIZE_Y);
}
