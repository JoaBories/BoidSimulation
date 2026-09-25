#pragma once

#include "Util.h"
#include <vector>
#include <string>

using Struct::Vec2F;
using Struct::Vec2I;

class Terrain
{
private:
	std::vector<std::vector<uint32_t>> mCostGrid;
	std::vector<std::vector<uint8_t>> mMap;
	Texture mMapTexture;
	Texture mCostTexture;
	Vec2I mDestination;
	Vec2I mSize;
	uint32_t mMaxCost;
	
	uint64_t mTotalDijkstraTime;
	uint32_t mDijkstraNumber;
	
	void clearCostGrid();
	void loadCostTexture() const;
	void rebuildCostGrid();

public:
	explicit Terrain(const std::string& imagePath);
	
	void newDestination(const Vec2I& destination);
	
	[[nodiscard]] bool isWalkable(const Vec2I& pos, uint8_t threshold = 128) const;
	
	Vec2I getSize() const { return mSize; }
	
	void update();
	void draw() const;
};

