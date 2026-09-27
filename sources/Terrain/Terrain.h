#pragma once

#include "Util.h"

#include <string>
#include "Grid2D.h"

class Terrain
{
private:
	Grid2D<Vec2F> mFlowField; // y major
	Grid2D<uint16_t> mCostGrid; // y major
	Grid2D<uint8_t> mMap; // y major
	
	Texture mMapTexture;
	Texture mDebugTexture;
	
	Vec2I mDestination;
	uint32_t mMaxCost;
	
	uint64_t mTotalDijkstraTime;
	uint32_t mDijkstraNumber;
	
	void buildCostGrid();
	void loadCostTexture() const;
	
	void buildFlowField();
	void loadFlowFieldTexture() const;

public:
	explicit Terrain(const std::string& imagePath);
	
	void bench(uint32_t tryNumber);
	
	void newDestination(const Vec2I& destination);
	
	[[nodiscard]] bool isWalkable(const Vec2I& pos, uint8_t threshold = 128) const;
	[[nodiscard]] bool isWalkable(const uint64_t& it, uint8_t threshold = 128) const;

	[[nodiscard]] Vec2I getSize() const { return mMap.getSize(); }
	
	void update();
	void draw() const;
};

