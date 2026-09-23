#include "BoidGrid.h"

BoidGrid::BoidGrid()
{
	mGrid.fill(std::array<std::vector<int>, sizeY>());

	for (size_t y = 0; y < sizeY; y++)
	{
		mGrid[y].fill(std::vector<int>());
	}
}

void BoidGrid::updateGrid(const std::vector<Vect2F>& boidPositions)
{
	const Vect2F gridSize = getGridSize();

	for (size_t y = 0; y < sizeY; y++)
	{
		for (size_t x = 0; x < sizeX; x++)
		{
			mGrid[y][x].clear();
		}
	}

	for (uint32_t i = 0; i < boidPositions.size(); i++)
	{
		Vect2F pos = boidPositions[i];

		size_t x = Math::Clamp<int>(boidPositions[i].x / gridSize.x, 0, sizeX - 1);
		size_t y = Math::Clamp<int>(boidPositions[i].y / gridSize.y, 0, sizeY - 1);

		mGrid[y][x].emplace_back(i);
	}
}

const std::vector<int>& BoidGrid::getNeighbors(const Vect2F& position, float maxRange)
{
	return {};
}

const Vect2I& BoidGrid::getGridPos(const Vect2F& position) const
{
	const Vect2F gridSize = getGridSize();

	size_t x = Math::Clamp<int>(position.x / gridSize.x, 0, sizeX - 1);
	size_t y = Math::Clamp<int>(position.y / gridSize.y, 0, sizeY - 1);

	return Vect2I(x, y);
}

const Vect2F& BoidGrid::getGridSize() const
{
	const Vect2F screenBounds(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()));
	return screenBounds / Vect2F(sizeX, sizeY);
}
