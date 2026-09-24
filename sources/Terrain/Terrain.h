#pragma once

#include "Util.h"
#include <vector>
#include <string>

using Struct::Vec2F;
using Struct::Vec2I;

class Terrain
{
private:
	std::vector<std::vector<uint8_t>> mMap;
	Texture mMapTexture;
	
	std::vector<std::vector<uint32_t>> mCostGrid;
	uint32_t mMaxCost;
	Texture mCostTexture;
	
	Vec2I tempDestination;
	
	Vec2I mSize;

public:
	explicit Terrain(const std::string& imagePath);

	void clearDijkstra();
	void loadCostTexture();
	
	void inverseDijkstra(Vec2I destination);
	[[nodiscard]] bool isWalkable(const Vec2I& pos, uint8_t threshold = 128) const;
	
	void update();
	void draw() const;
};

