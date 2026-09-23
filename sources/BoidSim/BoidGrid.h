#pragma once

#include <array>
#include <vector>

#include <Util.h>
using Struct::Vect2F;
using Struct::Vect2I;

constexpr int SIZE_Y = 10;
constexpr int SIZE_X = 10;

class BoidGrid
{
private:
	std::array<std::array<std::vector<int>, SIZE_X>, SIZE_Y> mGrid;

public:
	BoidGrid();

	void updateGrid(const std::vector<Vect2F>& boidPositions);

	[[nodiscard]] const std::vector<int>& getNeighbors(const Vect2I& gridPosition) const;

	[[nodiscard]] Vect2I getGridPos(const Vect2F& position) const;
	[[nodiscard]] Vect2F getGridSize() const;
};