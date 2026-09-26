#include "BoidManager.h"

#include <chrono>
#include <string>

void BoidManager::resolveVelocity()
{
    if (mBoidNumber == 0) return;
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        if (mBoidVelocities[i].sqrLength() > MAX_SPEED * MAX_SPEED) mBoidVelocities[i] = mBoidVelocities[i].normalized() * MAX_SPEED;
        mBoidPositions[i] += mBoidVelocities[i] * GetFrameTime();
    }
}

void BoidManager::checkScreenBounds()
{
    if (mBoidNumber == 0) return;
    
    const Vec2F screenBounds(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()));
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        if (mBoidPositions[i].x < 0)
        {
            mBoidPositions[i].x = 0;
            mBoidVelocities[i].x = -mBoidVelocities[i].x;
        }
        else if (mBoidPositions[i].x > screenBounds.x)
        {
            mBoidPositions[i].x = screenBounds.x;
            mBoidVelocities[i].x = -mBoidVelocities[i].x;
        }
        
        if (mBoidPositions[i].y < 0)
        {
            mBoidPositions[i].y = 0;
            mBoidVelocities[i].y = -mBoidVelocities[i].y;
        }
        else if (mBoidPositions[i].y > screenBounds.y)
        {
            mBoidPositions[i].y = screenBounds.y;
            mBoidVelocities[i].y = -mBoidVelocities[i].y;
        }
    }
}

void BoidManager::applyRules()
{
    if (mBoidNumber == 0) return;
    
    const float dotProductTreshold = -(PERCEPTION_ANGLE / 180.0f - 1.0f);
    const float higherRange = Math::max(SEPARATE_RANGE, Math::max(ALIGN_RANGE, GROUP_RANGE));

    const int stepX = (int)std::ceil(higherRange / mGrid.getGridSize().x);
    const int stepY = (int)std::ceil(higherRange / mGrid.getGridSize().y);
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        Vec2F separate { 0.0f, 0.0f };
        
        Vec2F alignSum { 0.0f, 0.0f };
        uint32_t alignCount = 0;
        
        Vec2F groupSum { 0.0f, 0.0f };
        uint32_t groupCount = 0;
        
        Vec2I gridPos = mGrid.getGridPos(mBoidPositions[i]);
        
        for (int y = -stepY; y < stepY+1; y++)
        {
            if (gridPos.y + y < 0 || gridPos.y + y >= GRID_SIZE) continue;

            for (int x = -stepX; x < stepX+1; x++)
            {
                if (gridPos.x + x < 0 || gridPos.x + x >= GRID_SIZE) continue;


                for (const uint32_t j : mGrid.getNeighbors(gridPos + Vec2I(x,y)))
                {
                    if (i == j) continue;

                    Vec2F distance = mBoidPositions[j] - mBoidPositions[i];
                    if (distance.dot(mBoidVelocities[i]) <= dotProductTreshold) // Skip neighbors not in view
                    {
                        continue;
                    }

                    const float distanceSquared = distance.dot(distance);
                    if (distanceSquared >= higherRange * higherRange) // Skip neighbors not in range
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
            group = (groupSum - mBoidPositions[i]).normalized();
        }
        
        Vec2F force = separate * BOID_WEIGHTS.separate + align * BOID_WEIGHTS.align + group * BOID_WEIGHTS.group;
        mBoidVelocities[i] += force * GetFrameTime();
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

    const float higherRange = Math::max(SEPARATE_RANGE, Math::max(ALIGN_RANGE, GROUP_RANGE));
    const int stepX = (int)std::ceil(higherRange / mGrid.getGridSize().x);
    const int stepY = (int)std::ceil(higherRange / mGrid.getGridSize().y);

    const Vec2I gridPos = mGrid.getGridPos(mBoidPositions[boidIndex]);

    for (int y = -stepY; y < stepY + 1; y++)
    {
        if (gridPos.y + y < 0 || gridPos.y + y >= GRID_SIZE) continue;

        for (int x = -stepX; x < stepX + 1; x++)
        {
            if (gridPos.x + x < 0 || gridPos.x + x >= GRID_SIZE) continue;


            for (const uint32_t j : mGrid.getNeighbors(gridPos + Vec2I(x, y)))
            {
                if (boidIndex == j) continue;

                DrawCircleLines((int)mBoidPositions[j].x, (int)mBoidPositions[j].y, 20.0f, BLACK);
            }
        }
    }
}

BoidManager::BoidManager(const uint32_t agentNumber) :
    mBoidNumber(agentNumber)
{
    init();
}

BoidManager::~BoidManager()
{
    mBoidPositions.clear();
    mBoidVelocities.clear();
}

void BoidManager::init()
{
    const Vec2F screenBounds(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()));
    
    mBoidPositions.reserve(mBoidNumber);
    mBoidVelocities.reserve(mBoidNumber);
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        mBoidPositions.emplace_back(Math::randFloat(0, screenBounds.x), Math::randFloat(0, screenBounds.y));
        mBoidVelocities.emplace_back(randVec2() * MAX_SPEED);
    }
}

void BoidManager::update()
{
    const auto start = std::chrono::high_resolution_clock::now();
    applyRules();
    resolveVelocity();
    checkScreenBounds();

    const auto gridStart = std::chrono::high_resolution_clock::now();
    mGrid.updateGrid(mBoidPositions);
    const auto gridEnd = std::chrono::high_resolution_clock::now();
    
    const auto end = std::chrono::high_resolution_clock::now();

    mLastUpdateTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    mLastGridUpdateTime += std::chrono::duration_cast<std::chrono::microseconds>(gridEnd - gridStart).count();
    mUpdateCount++;
}

void BoidManager::draw() const
{
    if (mBoidNumber == 0) return;
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        DrawCircleV(mBoidPositions[i].toRaylib(), 2.0f, DARKGRAY);
    }
    
    //draw debug
    //drawDebug(0);

    const std::string updateTime = "average update time : " + std::to_string((float)mLastUpdateTime / (float)mUpdateCount / 1000.0f) + " ms";
    const std::string boidTime = "average boid time : " + std::to_string((float)mLastUpdateTime / (float)mBoidNumber / (float)mUpdateCount) + " us";
    const std::string gridTime = "average grid time : " + std::to_string((float)mLastGridUpdateTime / (float)mUpdateCount) + " us";
    
    DrawText(updateTime.c_str(), 10, 30, 20, DARKGREEN);
    DrawText(boidTime.c_str(), 10, 50, 20, DARKGREEN);
    DrawText(gridTime.c_str(), 10, 70, 20, DARKGREEN);
}

#include "Util.h"

void BoidManager::logAverageUpdate()
{
    std::string updateTime = "average update time : " + std::to_string((float)mLastUpdateTime / (float)mUpdateCount / 1000.0f) + " ms";
    std::string boidTime = "average boid time : " + std::to_string((float)mLastUpdateTime / (float)mBoidNumber / (float)mUpdateCount) + " us";
    
    std::cout << updateTime << '\n';
    std::cout << boidTime << '\n';
}
