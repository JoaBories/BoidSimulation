#pragma once

#include <Util.h>
#include "Terrain/Grid2D.h"

class BoidGrid
{
private:
	Grid2D<std::vector<int>> mGrid;
	Vec2I mGridSize;
	Vec2F mCellSize;

public:
	explicit BoidGrid(const Vec2I& gridSize);
	
	void resize(const Vec2I& gridSize);

	void updateGrid(const std::vector<Vec2F>& boidPositions);

	[[nodiscard]] const std::vector<int>& getNeighbors(const Vec2I& gridPosition) const;

	[[nodiscard]] Vec2I getGridPos(const Vec2F& position) const;
	[[nodiscard]] const Vec2F& getCellSize() const { return mCellSize; }
	[[nodiscard]] const Vec2I& getGridSize() const { return mGridSize; }
};