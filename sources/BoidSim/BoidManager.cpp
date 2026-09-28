#include "BoidManager.h"

#include <algorithm>
#include <chrono>
#include <string>
#include <thread>

void BoidManager::resolveMovement(const size_t index)
{
    if (mVel.read(index).sqrLength() > MAX_SPEED * MAX_SPEED) mVel.write(index, mVel.read(index).normalized() * MAX_SPEED);
    else mVel.write(index, mVel.read(index));
    mPos.write(index, mPos.read(index) + mVel.read(index) * GetFrameTime());
}

void BoidManager::checkScreenBounds(const size_t index)
{
    if (mPos.read(index).x < 0)
    {
        mPos.write(index, { 0.0f , mPos.read(index).y } );
        mVel.write(index, mVel.read(index) * Vec2F(-1.0f, 1.0f) );
    }
    else if (mPos.read(index).x > mScreenBounds.x)
    {
        mPos.write(index, { mScreenBounds.x , mPos.read(index).y } );
        mVel.write(index, mVel.read(index) * Vec2F(-1.0f, 1.0f));
    }
    
    if (mPos.read(index).y < 0)
    {
        mPos.write(index, { mPos.read(index).x, 0.0f } );
        mVel.write(index, mVel.read(index) * Vec2F(1.0f, -1.0f));
    }
    else if (mPos.read(index).y > mScreenBounds.y)
    {
        mPos.write(index, { mPos.read(index).x, mScreenBounds.y } );
        mVel.write(index, mVel.read(index) * Vec2F(1.0f, -1.0f));
    }
}

void BoidManager::applyRules(const size_t index)
{
    const Vec2I gridPos = mGrid.getGridPos(mPos.read(index));

    Vec2F separate { 0.0f, 0.0f };

    Vec2F alignSum { 0.0f, 0.0f };
    uint32_t alignCount = 0;

    Vec2F groupSum { 0.0f, 0.0f };
    uint32_t groupCount = 0;
    
    float density = 0.0f;

    for (int y = -mCellStep.y; y < mCellStep.y+1; y++)
    {
        if (gridPos.y + y < 0 || gridPos.y + y >= mGrid.getGridSize().y) continue;

        for (int x = -mCellStep.x; x < mCellStep.x+1; x++)
        {
            if (gridPos.x + x < 0 || gridPos.x + x >= mGrid.getGridSize().x) continue;


            for (const uint32_t j : mGrid.getNeighbors(gridPos + Vec2I(x,y)))
            {
                if (index == j) continue;

                Vec2F distance = mPos.read(j) - mPos.read(index);
                const float distanceSquared = distance.dot(distance);
            
                if (distanceSquared >= HIGHER_RANGE * HIGHER_RANGE) // Skip neighbors not in range
                {
                    continue;
                }
            
                density += 1.0f;
            
                if (distance.dot(mVel.read(index)) <= DOT_PRODUCT_THRESHOLD) // Skip neighbors not in view
                {
                    continue;
                }

                if (distanceSquared <= SEPARATE_RANGE * SEPARATE_RANGE)
                {
                    separate += -distance / distanceSquared;
                }

                if (distanceSquared <= ALIGN_RANGE * ALIGN_RANGE)
                {
                    alignSum += mVel.read(j);
                    alignCount++;
                }

                if (distanceSquared <= GROUP_RANGE * GROUP_RANGE)
                {
                    groupSum += mPos.read(j);
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
        group = (groupSum - mPos.read(index)).normalized();
    }

    const Vec2F force = separate * BOID_WEIGHTS.separate + align * BOID_WEIGHTS.align + group * BOID_WEIGHTS.group;
    mVel.write(index, mVel.read(index) + force * GetFrameTime());
    mDensity.write(index, density);
}

void BoidManager::updateBoids()
{
    if (mBoidNumber == 0) return;
    
    std::vector<std::thread> workers;
    const size_t chunkSize = (size_t)std::ceil((float)(mBoidNumber - 1) / (float)mThreadNumber);
    
    for (size_t thread = 0; thread < mThreadNumber; thread++)
    {
        const size_t begin = thread * chunkSize;
        const size_t end = Math::min(begin + chunkSize, mBoidNumber);
        if (begin >= end) break;
        
        workers.emplace_back([&, begin, end]
        {
            for (size_t i = begin; i < end; i++)
            {
                applyRules(i); // write all vel and density
            }
        });
    }
    for (auto& w : workers) w.join();   // sync point
    workers.clear();

    mVel.swap();     // need a "write all" to swap otherwise it will swap with outdated data.
    mDensity.swap();
    
    // Without two distinct passes the comportment is bad because it updates position with 1 frame old velocity
    for (size_t thread = 0; thread < mThreadNumber; thread++)
    {
        const size_t begin = thread * chunkSize;
        const size_t end = Math::min(begin + chunkSize, mBoidNumber);
        if (begin >= end) break;
        
        workers.emplace_back([&, begin, end]
        {
            for (size_t i = begin; i < end; i++)
            {
                resolveMovement(i); // write all vel and pos
                checkScreenBounds(i); // update vel and pos
            }
        });
    }
    for (auto& w : workers) w.join();   // sync point
    workers.clear();

    mPos.swap();
    mVel.swap();
    
    mMaxDensity = 0;
    for (size_t index = 0; index < mBoidNumber; index++) mMaxDensity = std::max(mDensity.read(index), mMaxDensity);
}

void BoidManager::drawDebug(size_t boidIndex) const
{
    DrawLineEx(mPos.read(boidIndex).toRaylib(), (mPos.read(boidIndex) + mVel.read(boidIndex) * 1).toRaylib(), 1.0f, GREEN);

    const float perceptionEdgeR = mVel.read(boidIndex).getRot() - PERCEPTION_ANGLE / 2;
    const float perceptionEdgeL = mVel.read(boidIndex).getRot() + PERCEPTION_ANGLE / 2;
    
    DrawCircleSectorLines(mPos.read(boidIndex).toRaylib(), GROUP_RANGE, perceptionEdgeR, perceptionEdgeL, 10, PINK);
    DrawCircleSectorLines(mPos.read(boidIndex).toRaylib(), ALIGN_RANGE, perceptionEdgeR, perceptionEdgeL, 10, ORANGE);
    DrawCircleSectorLines(mPos.read(boidIndex).toRaylib(), SEPARATE_RANGE, perceptionEdgeR, perceptionEdgeL, 10, RED);
    
    const Vec2I gridPos = mGrid.getGridPos(mPos.read(boidIndex));

    for (int y = -mCellStep.y; y < mCellStep.y + 1; y++)
    {
        if (gridPos.y + y < 0 || gridPos.y + y >= mGrid.getGridSize().y) continue;

        for (int x = -mCellStep.x; x < mCellStep.x + 1; x++)
        {
            if (gridPos.x + x < 0 || gridPos.x + x >= mGrid.getGridSize().x) continue;


            for (const uint32_t j : mGrid.getNeighbors(gridPos + Vec2I(x, y)))
            {
                if (boidIndex == j) continue;
                DrawCircleLines((int)mPos.read(j).x, (int)mPos.read(j).y, 20.0f, BLACK);
            }
        }
    }
}

BoidManager::BoidManager(const size_t agentNumber) :
    mBoidNumber(agentNumber), mGrid(Vec2I::Zero),
    mScreenBounds((float)GetScreenWidth(), (float)GetScreenHeight()), 
    mTotalUpdateTime(0), mUpdateCount(0), mLogTime(0.0f),
    mMaxDensity(0.0f)
{
    mThreadNumber = Math::max(std::thread::hardware_concurrency(), 1u);
    
    const Vec2I gridSize = Vec2I::One + (mScreenBounds / HIGHER_RANGE).to<int>();
    mGrid.resize(gridSize);
    
    const int stepX = (int)std::ceil(HIGHER_RANGE / mGrid.getCellSize().x / 2.0f);
    const int stepY = (int)std::ceil(HIGHER_RANGE / mGrid.getCellSize().y / 2.0f);
    mCellStep = Vec2I(stepX, stepY);
    
    mPos.resize(mBoidNumber);
    mVel.resize(mBoidNumber);
    mDensity.resize(mBoidNumber);
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        mPos.write(i, { Math::randFloat(0, mScreenBounds.x), Math::randFloat(0, mScreenBounds.y) });
        mVel.write(i, randVec2() * MAX_SPEED);
        mDensity.write(i, 0.0f);
    }
    
    mPos.copy();
    mVel.copy();
    mDensity.copy();
    
    mGrid.updateGrid(mPos.readVector());
}

void BoidManager::update()
{
    mGrid.updateGrid(mPos.readVector());
    
    if constexpr (DEBUG_PERF)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        updateBoids();
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
            const Color color = colorLerp(YELLOW, RED, mDensity.read(i) / mMaxDensity);
            DrawCircleV(mPos.read(i).toRaylib(), 1.0f, color);
        }
    }
    else
    {
        for (uint32_t i = 0; i < mBoidNumber; i++)
        {
            DrawCircleV(mPos.read(i).toRaylib(), 1.0f, RAYWHITE);
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
