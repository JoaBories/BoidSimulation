#include "Terrain.h"

#include <chrono>
#include <queue>

Terrain::Terrain(const std::string& imagePath) :
	mDebugTexture(), mMaxCost(0)
{
	Image tempImg = LoadImage(imagePath.c_str());
	
	mSize = {tempImg.width, tempImg.height};
	mItSize = (size_t)mSize.x * mSize.y;
	
	mMap.resize(mItSize);
	for (size_t it = 0; it < mItSize; it++)
	{
		const Vec2I pos = getPosFromIterator(it);
		const uint8_t intensity = GetImageColor(tempImg, pos.x, pos.y).r;
		mMap[it] = intensity;
	}
	
	mMapTexture = LoadTextureFromImage(tempImg);
	UnloadImage(tempImg);
	
	tempImg = GenImageColor(mSize.x, mSize.y, BLANK);
	mDebugTexture = LoadTextureFromImage(tempImg);
	UnloadImage(tempImg);
	
}

void Terrain::bench(const uint32_t tryNumber)
{
	for (uint32_t i = 0; i < tryNumber + 1; i++)
	{
		Vec2I randomPos;
		do { randomPos = Vec2I(Math::randInt(0, mSize.x), Math::randInt(0, mSize.y)); } 
		while (!isWalkable(randomPos));
		
		newDestination(randomPos);
	}
}

void Terrain::newDestination(const Vec2I& destination)
{
	if (!isWalkable(destination)) return;
	mDestination = destination;

	const auto dijkstraStart = std::chrono::high_resolution_clock::now();
	buildCostGrid();
	const auto dijkstraEnd = std::chrono::high_resolution_clock::now();
	
	const auto flowStart = std::chrono::high_resolution_clock::now();
	buildFlowField();
	const auto flowEnd = std::chrono::high_resolution_clock::now();

	//loadCostTexture();
	loadFlowFieldTexture();
	
	mTotalDijkstraTime += std::chrono::duration_cast<std::chrono::microseconds>(dijkstraEnd - dijkstraStart).count();
	const auto flowTime = std::chrono::duration_cast<std::chrono::microseconds>(flowEnd - flowStart).count();
	mDijkstraNumber++;
	
	std::cout << "====" << mDijkstraNumber << "==========================================" << '\n';
	std::cout << "Average Dijkstra Time : " << std::to_string((float)mTotalDijkstraTime / (float)mDijkstraNumber / 1000.0f) << " ms" << '\n';
	std::cout << "Flow field Time : " << std::to_string((float)flowTime / 1000.0f) << " ms" << '\n';
}

void Terrain::loadCostTexture() const
{
	Image img = GenImageColor(mSize.x, mSize.y, BLANK);
	
	for (size_t it = 0; it < mItSize; it++)
	{
		if (mCostGrid[it] == 0) continue; // Not visited leave blank
		
		const Vec2I pos = getPosFromIterator(it);
		const float t = (float)mCostGrid[it] / (float)mMaxCost;
		ImageDrawPixel(&img, pos.x, pos.y, Struct::colorLerp(YELLOW, RED, t));
	}

	UpdateTexture(mDebugTexture, img.data);
	UnloadImage(img);
}

typedef std::pair<uint32_t, Vec2I> CellCostPos;

namespace
{
	struct CellComp
	{
		bool operator()(const CellCostPos& a, const CellCostPos& b) const noexcept { return a.first > b.first; }
	};
}

void Terrain::buildCostGrid()
{
	std::priority_queue<CellCostPos, std::vector<CellCostPos>, CellComp> cellsToVisit;
	uint32_t currentCost = 1;
	
	mCostGrid.clear();
	mCostGrid.resize(mItSize);
	
	mCostGrid[getIteratorFromPos(mDestination)] = currentCost;
	cellsToVisit.emplace(currentCost, mDestination);
	
	while (!cellsToVisit.empty())
	{
		auto [cost, pos] = cellsToVisit.top();
		cellsToVisit.pop();
		
		currentCost = cost;
		
		for (int x = -1; x < 2; x++) // Iterate through all neighbor cells.
		{
			if (pos.x + x < 0 || pos.x + x >= mSize.x) continue; // Eliminate cell that aren't in the board.
			
			for (int y = -1; y < 2; y++) // Iterate through all neighbor cells.
			{
				if (x == 0 && y == 0) continue; // Eliminate cell that aren't in the board.
				if (pos.y + y < 0 || pos.y + y >= mSize.y) continue; // Eliminate in the same cell.

				if (const Vec2I newCell = pos + Vec2I{ x, y }; isWalkable(newCell) && mCostGrid[getIteratorFromPos(newCell)] == 0)
				{
					const uint32_t newCost = currentCost + (x == 0 || y == 0 ? 5 : 7);
					
					mCostGrid[getIteratorFromPos(newCell)] = newCost;
					cellsToVisit.emplace(newCost ,newCell);
				}
			}
		}
	}
	
	mMaxCost = currentCost;
}

void Terrain::buildFlowField()
{
	mFlowField.clear();
	mFlowField.resize(mItSize);
	
	for (size_t it = 0; it < mItSize; it++)
	{
		if (!isWalkable(it)) continue;
		
		Vec2I pos = getPosFromIterator(it);
		
		int gX = 0;
		int gY = 0;
		
		for (int x = -1; x < 2; x++)
		{
			if (pos.x + x < 0 || pos.x + x >= mSize.x) continue; // Eliminate cell that aren't in the board.
			
			for (int y = -1; y < 2; y++)
			{
				if (x == 0 && y == 0) continue;
				if (pos.y + y < 0 || pos.y + y >= mSize.y) continue; // Eliminate cell that aren't in the board.
				
				Vec2I newPos = pos + Vec2I{ x, y };
				if (!isWalkable(newPos)) continue;

				const int cost = (int)mCostGrid[getIteratorFromPos(newPos)];
				gX += cost * x * (y == 0 ? 2 : 1);
				gY += cost * y * (x == 0 ? 2 : 1);
			}
		}
		
		mFlowField[it] = -Vec2F( (float)gX, (float)gY ).normalized();
	}
}

void Terrain::loadFlowFieldTexture() const
{
	Image img = GenImageColor(mSize.x, mSize.y, BLANK);
	
	for (size_t it = 0; it < mItSize; it++)
	{
		if (mFlowField[it] == Vec2F::Zero) continue; // Not visited leave blank
		
		const Vec2I pos = getPosFromIterator(it);
		const Vec2F dir = (mFlowField[it] + Vec2F::One) / 2.0f;
		ImageDrawPixel(&img, pos.x, pos.y, { (uint8_t)(dir.x * 255.0f), (uint8_t)(dir.y * 255.0f), 0 , 255 });
	}

	UpdateTexture(mDebugTexture, img.data);
	UnloadImage(img);
}

bool Terrain::isWalkable(const Vec2I& pos, const uint8_t threshold) const
{
	if (pos.x < 0 || pos.x >= mSize.x) return false;
	if (pos.y < 0 || pos.y >= mSize.y) return false;
	
	return mMap[getIteratorFromPos(pos)] > threshold;
}

bool Terrain::isWalkable(const uint64_t it, uint8_t threshold) const
{
	if (it < mItSize) return mMap[it];
	return false;
}

void Terrain::update()
{
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		mDestination = Vec2I(GetMousePosition()) * mSize / Vec2I{ GetScreenWidth(), GetScreenHeight() };
		newDestination(mDestination);
	}
}

void Terrain::draw() const 
{
	const Vec2F screenSize{ (float)GetScreenWidth(), (float)GetScreenHeight() };
	
	const Rectangle sourceRect = { 0,0, (float)mSize.x, (float)mSize.y };
	const Rectangle destRect = { 0, 0, screenSize.x, screenSize.y};
	
	DrawTexturePro(mMapTexture, sourceRect, destRect, {0,0}, 0, WHITE);
	DrawTexturePro(mDebugTexture, sourceRect, destRect, {0,0}, 0, WHITE);

	const Vec2F destinationOnScreen = mDestination.to<float>() / mSize.to<float>() * screenSize;
	DrawCircleV(destinationOnScreen.toRaylib(), 5.0f, RED);
}
