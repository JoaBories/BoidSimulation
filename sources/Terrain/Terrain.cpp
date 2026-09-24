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

typedef std::pair<uint32_t, Vec2I> CellCostPos;

struct cellComp
{
	bool operator()(const CellCostPos& a, const CellCostPos& b) const noexcept { return a.first > b.first; }
};

void Terrain::inverseDijkstra(const Vec2I destination)
{
	std::priority_queue<CellCostPos, std::vector<CellCostPos>, cellComp> cellsToVisit;
	
	clearDijkstra();
	
	cellsToVisit.emplace(0, destination);
	mCostGrid[destination.y][destination.x] = 0;
	
	uint32_t currentCost = 0;
	
	while (!cellsToVisit.empty())
	{
		auto [cost, pos] = cellsToVisit.top();
		cellsToVisit.pop();
		
		currentCost = cost;
		
		for (int x = -1; x < 2; x++)
		{
			if (pos.x + x < 0 || pos.x + x >= mSize.x) continue;
			
			for (int y = -1; y < 2; y++)
			{
				if (x == 0 && y == 0) continue;
				if (pos.y + y < 0 || pos.y + y >= mSize.y) continue;

				const Vec2I newCell = pos + Vec2I{ x, y };
				if (isWalkable(newCell) && mCostGrid[newCell.y][newCell.x] == 0)
				{
					const uint32_t newCost = currentCost + (x == 0 || y == 0 ? 2 : 3);
					mCostGrid[newCell.y][newCell.x] = newCost;
					cellsToVisit.emplace(newCost ,newCell);
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
		
		mDestination = (mousePos * mSize.to<float>() / screenSize).to<int>();
		
		inverseDijkstra(mDestination);
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
	
	Vec2F destinationOnScreen = mDestination.to<float>() / mSize.to<float>() * screenSize;
	DrawCircleV(destinationOnScreen.toRaylib(), 5.0f, RED);
}
