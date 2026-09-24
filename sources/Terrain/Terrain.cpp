#include "Terrain.h"

#include <queue>

using Struct::Rect2;

Terrain::Terrain(const std::string& imagePath)
{
	const Image map = LoadImage(imagePath.c_str());
	
	mMap.resize(map.height);
	for (int y = 0; y < map.height; y++)
	{
		mMap[y].resize(map.width);
		for (int x = 0; x < map.width; x++)
		{
			const uint8_t intensity = GetImageColor(map, x, y).r;
			mMap[y][x] = intensity;
		}
	}
	
	mSize = {map.width, map.height};
	mMapTexture = LoadTextureFromImage(map);
	UnloadImage(map);
}

void Terrain::clearDijkstra()
{
	UnloadTexture(mCostTexture);
	mCostGrid.clear();
	
	mCostGrid.resize(mSize.y);
	for (int y = 0; y < mSize.y; y++)
	{
		mCostGrid[y].resize(mSize.x);
	}
}

void Terrain::loadCostTexture()
{
	Image img = GenImageColor(mSize.x, mSize.y, BLANK);
	
	for (int y = 0; y < mSize.y; y++)
	{
		for (int x = 0; x < mSize.x; x++)
		{
			Color costColor;
			
			if (mCostGrid[y][x] == 0)
			{
				costColor = BLANK;
			}
			else
			{
				const float t = (float)mCostGrid[y][x] / (float)mMaxCost;
				costColor = Struct::colorLerp(GREEN, RED, t);
			}
			ImageDrawPixel(&img, x, y, costColor);
		}
	}
	
	mCostTexture = LoadTextureFromImage(img);
	UnloadImage(img);
}

void Terrain::inverseDijkstra(const Vec2I destination)
{
	std::queue<Vec2I> cellsToVisit;
	
	clearDijkstra();
	
	cellsToVisit.push(destination);
	mCostGrid[destination.y][destination.x] = 0;
	
	uint32_t currentCost = 0;
	
	while (!cellsToVisit.empty())
	{
		Vec2I currentCell = cellsToVisit.front();
		cellsToVisit.pop();
		
		currentCost = mCostGrid[currentCell.y][currentCell.x];
		
		for (int x = -1; x < 2; x++)
		{
			if (currentCell.x + x < 0 || currentCell.x + x >= mSize.x) continue;
			
			for (int y = -1; y < 2; y++)
			{
				if (x == 0 && y == 0) continue;
				if (currentCell.y + y < 0 || currentCell.y + y >= mSize.y) continue;

				const Vec2I newCell = currentCell + Vec2I{ x, y };
				if (isWalkable(currentCell) && mCostGrid[newCell.y][newCell.x] == 0)
				{
					const uint32_t cost = currentCost + (x == 0 || y == 0 ? 2 : 3);
					mCostGrid[newCell.y][newCell.x] = cost;
					cellsToVisit.push(newCell);
				}
			}
		}
	}
	
	mMaxCost = currentCost;
	std::cout << std::to_string(currentCost) << '\n';
	
	loadCostTexture();
}

bool Terrain::isWalkable(const Vec2I& pos, const uint8_t threshold) const
{
	return mMap[pos.y][pos.x] > threshold;
}

void Terrain::update()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		const Vec2F screenSize{ (float)GetScreenWidth(), (float)GetScreenHeight() };
		const Vec2F mousePos = GetMousePosition();
		
		tempDestination = (mousePos * mSize.to<float>() / screenSize).to<int>();
		
		inverseDijkstra(tempDestination);
	}
}

void Terrain::draw() const 
{
	const Vec2F screenSize{ (float)GetScreenWidth(), (float)GetScreenHeight() };
	
	const Rectangle sourceRect = { 0,0, (float)mSize.x, (float)mSize.y };
	const Rectangle destRect = { 0, 0, screenSize.x, screenSize.y};
	
	DrawTexturePro(mMapTexture, sourceRect, destRect, {0,0}, 0, WHITE);
	
	if (mCostTexture.width != 0)
	{
		DrawTexturePro(mCostTexture, sourceRect, destRect, {0,0}, 0, WHITE);
	}
	
	Vec2F destinationOnScreen = tempDestination.to<float>() / mSize.to<float>() * screenSize;
	DrawCircleV(destinationOnScreen.toRaylib(), 5.0f, RED);
}
