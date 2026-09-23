#pragma once

#include <array>
#include <vector>

#include <Util.h>
using Struct::Vect2F;
using Struct::Vect2I;

constexpr size_t sizeY = 20;
constexpr size_t sizeX = 20;

class BoidGrid
{
private:
	std::array<std::array<std::vector<int>, sizeX>, sizeY> mGrid;
	Vect2F gridSize;

public:
	BoidGrid();

	void updateGrid(const std::vector<Vect2F>& boidPositions);

	const std::vector<int>& getNeighbors(const Vect2I& gridPosition) const;

	const Vect2I& getGridPos(const Vect2F& position) const;
	const Vect2F& getGridSize() const;
};