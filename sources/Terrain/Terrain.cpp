#include "Terrain.h"

#include <chrono>
#include <queue>

namespace
{
	typedef std::pair<uint16_t, Vec2I> CellCostPos;
	struct CellComp
	{
		bool operator()(const CellCostPos& a, const CellCostPos& b) const noexcept { return a.first > b.first; }
	};
}

void Terrain::buildCostGrid()
{
	std::priority_queue<CellCostPos, std::vector<CellCostPos>, CellComp> cellsToVisit;
	uint16_t currentCost = 1;
	
	mCostGrid.clear();
	
	mCostGrid[mDestination] = currentCost;
	cellsToVisit.emplace(currentCost, mDestination);
	
	while (!cellsToVisit.empty())
	{
		auto [cost, pos] = cellsToVisit.top();
		cellsToVisit.pop();
		
		currentCost = cost;
		
		for (int x = -1; x < 2; x++) // Iterate through all neighbor cells.
		{
			if (pos.x + x < 0 || pos.x + x >= mMap.getSize().x) continue; // Eliminate cell that aren't in the board.
			
			for (int y = -1; y < 2; y++) // Iterate through all neighbor cells.
			{
				if (x == 0 && y == 0) continue; // Eliminate cell that aren't in the board.
				if (pos.y + y < 0 || pos.y + y >= mCostGrid.getSize().y) continue; // Eliminate in the same cell.

				if (const Vec2I newCell = pos + Vec2I{ x, y }; isWalkable(newCell) && mCostGrid[newCell] == 0)
				{
					const uint16_t newCost = currentCost + (x == 0 || y == 0 ? 5 : 7);
					
					mCostGrid[newCell] = newCost;
					cellsToVisit.emplace(newCost ,newCell);
				}
			}
		}
	}
	
	mMaxCost = currentCost;
}

void Terrain::loadCostTexture() const
{
	Image img = GenImageColor(mCostGrid.getSize().x, mCostGrid.getSize().y, BLANK);
	
	for (size_t it = 0; it < mMap.getItSize(); it++)
	{
		if (mCostGrid[it] == 0) continue; // Not visited leave blank
		
		const Vec2I pos = mCostGrid.indexToVec2(it);
		const float t = (float)mCostGrid[it] / (float)mMaxCost;
		ImageDrawPixel(&img, pos.x, pos.y, colorLerp(YELLOW, RED, t));
	}

	UpdateTexture(mDebugTexture, img.data);
	UnloadImage(img);
}

void Terrain::buildFlowField()
{
	mFlowField.clear();
	
	for (size_t it = 0; it < mCostGrid.getItSize(); it++)
	{
		if (!isWalkable(it)) continue;

		const Vec2I pos = mCostGrid.indexToVec2(it);
		
		Vec2I kernelResult = Vec2I::Zero;
		
		for (int x = -1; x < 2; x++)
		{
			if (pos.x + x < 0 || pos.x + x >= mCostGrid.getSize().x) continue; // Eliminate cell that aren't in the board.
			
			for (int y = -1; y < 2; y++)
			{
				if (x == 0 && y == 0) continue;
				if (pos.y + y < 0 || pos.y + y >= mCostGrid.getSize().y) continue; // Eliminate cell that aren't in the board.
				
				Vec2I newPos = pos + Vec2I{ x, y };
				if (!isWalkable(newPos)) continue;

				const int cost = (int)mCostGrid[newPos];
				kernelResult += Vec2I{cost * x * (y == 0 ? 2 : 1), cost * y * (x == 0 ? 2 : 1)};
			}
		}
		
		mFlowField[it] = -kernelResult.to<float>().normalized();
	}
}

void Terrain::loadFlowFieldTexture() const
{
	Image img = GenImageColor(mFlowField.getSize().x, mFlowField.getSize().y, BLANK);
	
	for (size_t it = 0; it < mFlowField.getItSize(); it++)
	{
		if (mFlowField[it] == Vec2F::Zero) continue; // Not visited leave blank
		
		const Vec2I pos = mFlowField.indexToVec2(it);
		const Vec2F dir = (mFlowField[it] + Vec2F::One) / 2.0f;
		ImageDrawPixel(&img, pos.x, pos.y, { (uint8_t)(dir.x * 255.0f), (uint8_t)(dir.y * 255.0f), 0 , 255 });
	}

	UpdateTexture(mDebugTexture, img.data);
	UnloadImage(img);
}

Terrain::Terrain(const std::string& imagePath) :
	mFlowField(Vec2I::Zero), mCostGrid(Vec2I::Zero), mMap(Vec2I::Zero), 
	mMaxCost(0),
	mTotalDijkstraTime(0), mDijkstraNumber(0)
{
	Image tempImg = LoadImage(imagePath.c_str());

	const Vec2I size = {tempImg.width, tempImg.height};
	mFlowField.resize(size);
	mCostGrid.resize(size);
	mMap.resize(size);
	
	for (size_t it = 0; it < mMap.getItSize(); it++)
	{
		const Vec2I pos = mMap.indexToVec2(it);
		const uint8_t intensity = GetImageColor(tempImg, pos.x, pos.y).r;
		mMap[it] = intensity;
	}
	
	mMapTexture = LoadTextureFromImage(tempImg);
	UnloadImage(tempImg);
	
	tempImg = GenImageColor(size.x, size.y, BLANK);
	mDebugTexture = LoadTextureFromImage(tempImg);
	UnloadImage(tempImg);
}

void Terrain::bench(const uint32_t tryNumber)
{
	for (uint32_t i = 0; i < tryNumber + 1; i++)
	{
		Vec2I randomPos;
		do { randomPos = Vec2I(Math::randInt(0, mMap.getSize().x), Math::randInt(0, mMap.getSize().y)); } 
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

bool Terrain::isWalkable(const Vec2I& pos, const uint8_t threshold) const
{
	if (pos.x < 0 || pos.x >= mMap.getSize().x) return false;
	if (pos.y < 0 || pos.y >= mMap.getSize().y) return false;
	
	return mMap[pos] > threshold;
}

bool Terrain::isWalkable(const uint64_t& it, const uint8_t threshold) const
{
	if (it < mMap.getItSize()) return mMap[it] > threshold;
	return false;
}

void Terrain::update()
{
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		mDestination = Vec2I(GetMousePosition()) * mMap.getSize() / Vec2I{ GetScreenWidth(), GetScreenHeight() };
		newDestination(mDestination);
	}
}

void Terrain::draw() const 
{
	const Vec2F screenSize{ (float)GetScreenWidth(), (float)GetScreenHeight() };
	
	const Rectangle sourceRect = { 0,0, (float)mMap.getSize().x, (float)mMap.getSize().y };
	const Rectangle destRect = { 0, 0, screenSize.x, screenSize.y};
	
	DrawTexturePro(mMapTexture, sourceRect, destRect, {0,0}, 0, WHITE);
	DrawTexturePro(mDebugTexture, sourceRect, destRect, {0,0}, 0, WHITE);

	const Vec2F destinationOnScreen = mDestination.to<float>() / mMap.getSize().to<float>() * screenSize;
	DrawCircleV(destinationOnScreen.toRaylib(), 5.0f, RED);
}
