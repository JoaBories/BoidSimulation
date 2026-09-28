#include "BoidManager.h"

#include <chrono>
#include <string>

void BoidManager::resolveVelocity(const uint32_t boidIndex)
{
    if (mBoidVelocities[boidIndex].sqrLength() > MAX_SPEED * MAX_SPEED) mBoidVelocities[boidIndex] = mBoidVelocities[boidIndex].normalized() * MAX_SPEED;
    mBoidPositions[boidIndex] += mBoidVelocities[boidIndex] * GetFrameTime();
}

void BoidManager::checkScreenBounds(const uint32_t boidIndex)
{    
    if (mBoidPositions[boidIndex].x < 0)
    {
        mBoidPositions[boidIndex].x = BOID_WRAP ? mScreenBounds.x : 0;
        if constexpr (!BOID_WRAP) mBoidVelocities[boidIndex].x = -mBoidVelocities[boidIndex].x;
    }
    else if (mBoidPositions[boidIndex].x > mScreenBounds.x)
    {
        mBoidPositions[boidIndex].x = BOID_WRAP ? 0 : mScreenBounds.x;
        if constexpr (!BOID_WRAP) mBoidVelocities[boidIndex].x = -mBoidVelocities[boidIndex].x;
    }
    
    if (mBoidPositions[boidIndex].y < 0)
    {
        mBoidPositions[boidIndex].y = BOID_WRAP ? mScreenBounds.y : 0;
        if constexpr (!BOID_WRAP) mBoidVelocities[boidIndex].y = -mBoidVelocities[boidIndex].y;
    }
    else if (mBoidPositions[boidIndex].y > mScreenBounds.y)
    {
        mBoidPositions[boidIndex].y = BOID_WRAP ? 0 : mScreenBounds.y;
        if constexpr (!BOID_WRAP) mBoidVelocities[boidIndex].y = -mBoidVelocities[boidIndex].y;
    }
}

void BoidManager::applyRules(const uint32_t boidIndex)
{
    const Vec2I gridPos = mGrid.getGridPos(mBoidPositions[boidIndex]);
    
    Vec2F separate { 0.0f, 0.0f };
    
    Vec2F alignSum { 0.0f, 0.0f };
    uint32_t alignCount = 0;
    
    Vec2F groupSum { 0.0f, 0.0f };
    uint32_t groupCount = 0;
    
    mBoidDensities[boidIndex] = 0;
    
    for (int y = -mCellStep.y; y < mCellStep.y+1; y++)
    {
        if (gridPos.y + y < 0 || gridPos.y + y >= mGrid.getGridSize().y) continue;

        for (int x = -mCellStep.x; x < mCellStep.x+1; x++)
        {
            if (gridPos.x + x < 0 || gridPos.x + x >= mGrid.getGridSize().x) continue;


            for (const uint32_t j : mGrid.getNeighbors(gridPos + Vec2I(x,y)))
            {
                if (boidIndex == j) continue;

                Vec2F distance = mBoidPositions[j] - mBoidPositions[boidIndex];
                const float distanceSquared = distance.dot(distance);
                
                if (distanceSquared >= HIGHER_RANGE * HIGHER_RANGE) // Skip neighbors not in range
                {
                    continue;
                }
                
                mBoidDensities[boidIndex] += 1;
                
                if (distance.dot(mBoidVelocities[boidIndex]) <= DOT_PRODUCT_THRESHOLD) // Skip neighbors not in view
                {
                    continue;
                }

                if (distanceSquared <= SEPARATE_RANGE * SEPARATE_RANGE)
                {
                    separate += -distance / distanceSquared;
                }

                if (distanceSquared <= ALIGN_RANGE * ALIGN_RANGE)
                {
                    alignSum += mBoidVelocities[j];
                    alignCount++;
                }

                if (distanceSquared <= GROUP_RANGE * GROUP_RANGE)
                {
                    groupSum += mBoidPositions[j];
                    groupCount++;
                }
            }
        }
    }
    
    separate = separate.normalized();
    
    Vec2F align { 0.0f, 0.0f };
    if (alignCount)
    {
        alignSum /= static_cast<float>(alignCount);
        align = alignSum.normalized() * MAX_SPEED;
    }
    
    Vec2F group { 0.0f, 0.0f };
    if (groupCount)
    {
        groupSum /= static_cast<float>(groupCount);
        group = (groupSum - mBoidPositions[boidIndex]).normalized();
    }

    const Vec2F force = separate * BOID_WEIGHTS.separate + align * BOID_WEIGHTS.align + group * BOID_WEIGHTS.group;
    mBoidVelocities[boidIndex] += force * GetFrameTime();
    
    if (mMaxDensity < mBoidDensities[boidIndex]) mMaxDensity = mBoidDensities[boidIndex];
}

void BoidManager::updateBoids()
{
    if (mBoidNumber == 0) return;
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        applyRules(i);
        resolveVelocity(i);
        checkScreenBounds(i);
    }
}

void BoidManager::drawDebug(const uint32_t boidIndex) const
{
    DrawLineEx(mBoidPositions[boidIndex].toRaylib(), (mBoidPositions[boidIndex] + mBoidVelocities[boidIndex] * 1).toRaylib(), 1.0f, GREEN);

    const float perceptionEdgeR = mBoidVelocities[boidIndex].getRot() - PERCEPTION_ANGLE / 2;
    const float perceptionEdgeL = mBoidVelocities[boidIndex].getRot() + PERCEPTION_ANGLE / 2;
    
    DrawCircleSectorLines(mBoidPositions[boidIndex].toRaylib(), GROUP_RANGE, perceptionEdgeR, perceptionEdgeL, 10, PINK);
    DrawCircleSectorLines(mBoidPositions[boidIndex].toRaylib(), ALIGN_RANGE, perceptionEdgeR, perceptionEdgeL, 10, ORANGE);
    DrawCircleSectorLines(mBoidPositions[boidIndex].toRaylib(), SEPARATE_RANGE, perceptionEdgeR, perceptionEdgeL, 10, RED);
    
    const Vec2I gridPos = mGrid.getGridPos(mBoidPositions[boidIndex]);

    for (int y = -mCellStep.y; y < mCellStep.y + 1; y++)
    {
        if (gridPos.y + y < 0 || gridPos.y + y >= mGrid.getGridSize().y) continue;

        for (int x = -mCellStep.x; x < mCellStep.x + 1; x++)
        {
            if (gridPos.x + x < 0 || gridPos.x + x >= mGrid.getGridSize().x) continue;


            for (const uint32_t j : mGrid.getNeighbors(gridPos + Vec2I(x, y)))
            {
                if (boidIndex == j) continue;
                DrawCircleLines((int)mBoidPositions[j].x, (int)mBoidPositions[j].y, 20.0f, BLACK);
            }
        }
    }
}

BoidManager::BoidManager(const uint32_t agentNumber) :
    mBoidNumber(agentNumber), mGrid(Vec2I::Zero),
    mScreenBounds((float)GetScreenWidth(), (float)GetScreenHeight()), 
    mTotalUpdateTime(0), mUpdateCount(0), mLogTime(0.0f),
    mMaxDensity(0.0f)
{
    const Vec2I gridSize = Vec2I::One + (mScreenBounds / HIGHER_RANGE).to<int>();
    mGrid.resize(gridSize);
    
    const int stepX = (int)std::ceil(HIGHER_RANGE / mGrid.getCellSize().x / 2.0f);
    const int stepY = (int)std::ceil(HIGHER_RANGE / mGrid.getCellSize().y / 2.0f);
    mCellStep = Vec2I(stepX, stepY);
    
    mBoidPositions.reserve(mBoidNumber);
    mBoidVelocities.reserve(mBoidNumber);
    mBoidDensities.resize(mBoidNumber);
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        mBoidPositions.emplace_back(Math::randFloat(0, mScreenBounds.x), Math::randFloat(0, mScreenBounds.y));
        mBoidVelocities.emplace_back(randVec2() * MAX_SPEED);
    }
    
    mGrid.updateGrid(mBoidPositions);
}

BoidManager::~BoidManager()
{
    mBoidPositions.clear();
    mBoidVelocities.clear();
}

void BoidManager::update()
{
    mMaxDensity = 0;

    if constexpr (DEBUG_PERF)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        updateBoids();
        mGrid.updateGrid(mBoidPositions);
        const auto end = std::chrono::high_resolution_clock::now();
        
        mTotalUpdateTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        mUpdateCount++;
        
        mLogTime += GetFrameTime();
        if (mLogTime >= 60.0f)
        {
            logAverageUpdate();
            mLogTime = 0.0f;
        }
    }
    else
    {
        updateBoids();
        mGrid.updateGrid(mBoidPositions);
    }
}

void BoidManager::draw() const
{
    if (mBoidNumber == 0) return;
    
    if constexpr (DEBUG_DENSITY)
    {
        mGrid.draw();
    
        for (uint32_t i = 0; i < mBoidNumber; i++)
        {
            const Color color = colorLerp(YELLOW, RED, mBoidDensities[i] / mMaxDensity);
            DrawCircleV(mBoidPositions[i].toRaylib(), 1.0f, color);
        }
    }
    else
    {
        for (uint32_t i = 0; i < mBoidNumber; i++)
        {
            DrawCircleV(mBoidPositions[i].toRaylib(), 1.0f, RAYWHITE);
        }
    }
    
    //drawDebug(0); //Uncomment to debug one boid.

    if constexpr (DEBUG_PERF)
    {
        const std::string updateTime = "average update time : " + std::to_string((float)mTotalUpdateTime / (float)mUpdateCount / 1000.0f) + " ms";
        const std::string boidTime = "average boid time : " + std::to_string((float)mTotalUpdateTime / (float)mBoidNumber / (float)mUpdateCount) + " us";
        const std::string maxDensity = "max density : " + std::to_string(mMaxDensity);
    
        DrawText(updateTime.c_str(), 10, 30, 20, GREEN);
        DrawText(boidTime.c_str(), 10, 50, 20, GREEN);
        DrawText(maxDensity.c_str(), 10, 70, 20, GREEN);
    }
}

void BoidManager::logAverageUpdate() const
{
    const std::string updateTime = "average update time : " + std::to_string((float)mTotalUpdateTime / (float)mUpdateCount / 1000.0f) + " ms";
    const std::string boidTime = "average boid time : " + std::to_string((float)mTotalUpdateTime / (float)mBoidNumber / (float)mUpdateCount) + " us";
    
    std::cout << updateTime << '\n';
    std::cout << boidTime << '\n';
}
