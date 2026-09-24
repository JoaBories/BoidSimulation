#pragma once

#include "Util.h"
#include <vector>
#include <string>

using Struct::Vec2F;

class Terrain
{
private:
	std::vector<std::vector<uint8_t>> mMap;
	Texture mMapTexture;
	Vec2F mSize;

public:
	Terrain(std::string imagePath);

	void draw() const;
};

