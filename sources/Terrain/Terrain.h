#pragma once

#include "Util.h"
#include <vector>
#include <string>

using Struct::Vec2F;
using Struct::Vec2I;

class Terrain
{
private:
	std::vector<Vec2F> mFlowField; // y major
	std::vector<uint32_t> mCostGrid; // y major
	Texture mDebugTexture;
	
	std::vector<uint8_t> mMap; // y major
	Texture mMapTexture;
	
	Vec2I mSize;
	size_t mItSize;
	
	Vec2I mDestination;
	uint32_t mMaxCost;
	
	uint64_t mTotalDijkstraTime;
	uint32_t mDijkstraNumber;
	
	void loadCostTexture() const;
	void buildCostGrid();
	
	void buildFlowField();
	void loadFlowFieldTexture() const;

	[[nodiscard]] constexpr uint64_t getIteratorFromPos(const Vec2I& pos) const { return pos.y * mSize.x + pos.x; }
	[[nodiscard]] constexpr Vec2I getPosFromIterator(uint64_t iterator) const { return Vec2I{ (int)iterator % mSize.y, (int)iterator / mSize.y }; }

public:
	explicit Terrain(const std::string& imagePath);
	
	void bench(uint32_t tryNumber);
	
	void newDestination(const Vec2I& destination);
	
	[[nodiscard]] bool isWalkable(const Vec2I& pos, uint8_t threshold = 128) const;
	[[nodiscard]] bool isWalkable(uint64_t it, uint8_t threshold = 128) const;
	
	Vec2I getSize() const { return mSize; }
	
	void update();
	void draw() const;
};

