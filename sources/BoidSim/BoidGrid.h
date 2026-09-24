#pragma once

#include <array>
#include <vector>

#include <Util.h>
using Struct::Vec2F;
using Struct::Vec2I;

constexpr int GRID_SIZE = 10;

class BoidGrid
{
private:
	std::array<std::array<std::vector<int>, GRID_SIZE>, GRID_SIZE> mGrid;

public:
	BoidGrid();

	void updateGrid(const std::vector<Vec2F>& boidPositions);

	[[nodiscard]] const std::vector<int>& getNeighbors(const Vec2I& gridPosition) const;

	[[nodiscard]] Vec2I getGridPos(const Vec2F& position) const;
	[[nodiscard]] Vec2F getGridSize() const;
};