#include "BoidManager.h"

#include <algorithm>
#include <chrono>
#include <string>
#include <thread>

void BoidManager::integrate(const size_t index)
{
    if (mVel.readNext(index).sqrLength() > MAX_SPEED * MAX_SPEED) mVel.writeNext(index, mVel.readNext(index).normalized() * MAX_SPEED);
    else if (mVel.readNext(index).sqrLength() < MAX_SPEED * MAX_SPEED) mVel.writeNext(index, mVel.readNext(index).normalized() * MIN_SPEED);
    else mVel.writeNext(index, mVel.readNext(index));
    mPos.writeNext(index, mPos.readCurrent(index) + mVel.readNext(index) * GetFrameTime());
    
    // readNext so it can read the updated velocity form apply rules
    // no possible data concurrency because it only reads own boid infos
    
    if (mPos.readNext(index).x < 0)
    {
        mPos.writeNext(index, { 0.0f , mPos.readNext(index).y } );
        mVel.writeNext(index, mVel.readNext(index) * Vec2F(-1.0f, 1.0f) );
    }
    else if (mPos.readNext(index).x > mScreenBounds.x)
    {
        mPos.writeNext(index, { mScreenBounds.x , mPos.readNext(index).y } );
        mVel.writeNext(index, mVel.readNext(index) * Vec2F(-1.0f, 1.0f));
    }
    
    if (mPos.readNext(index).y < 0)
    {
        mPos.writeNext(index, { mPos.readNext(index).x, 0.0f } );
        mVel.writeNext(index, mVel.readNext(index) * Vec2F(1.0f, -1.0f));
    }
    else if (mPos.readNext(index).y > mScreenBounds.y)
    {
        mPos.writeNext(index, { mPos.readNext(index).x, mScreenBounds.y } );
        mVel.writeNext(index, mVel.readNext(index) * Vec2F(1.0f, -1.0f));
    }
}

void BoidManager::applyRules(const size_t index)
{
    const Vec2F pos = mPos.readCurrent(index);
    const Vec2F vel = mVel.readCurrent(index);
    const Vec2I gridPos = mGrid.getGridPos(pos);

    Vec2F separateSum{};
    Vec2F alignSum{};
    Vec2F groupSum{};

    int separateCount{0};
    int alignCount{0};
    int groupCount{0};
    
    float density{0.0f};
    
    const int minX = std::max(0, gridPos.x - mCellStepX);
    const int maxX = std::min(mGrid.getGridSize().x, gridPos.x + mCellStepX + 1);
    
    const int minY = std::max(0, gridPos.y - mCellStepY);
    const int maxY = std::min(mGrid.getGridSize().y, gridPos.y + mCellStepY + 1);

    for (int y = minY; y < maxY; y++)
    {
        for (int x = minX; x < maxX; x++)
        {
            for (const uint32_t j : mGrid.getNeighbors(x,y))
            {
                if (index == j) continue;

                Vec2F distance = mPos.readCurrent(j) - pos;
                const float distanceSquared = distance.dot(distance);
            
                if (distanceSquared >= GROUP_RANGE * GROUP_RANGE) continue; // Skip neighbors not in range
            
                density += 1.0f;
            
                if (distance.dot(vel) <= DOT_PRODUCT_THRESHOLD || distanceSquared < 1e-6f) continue; // Skip neighbors not in view

                if (distanceSquared <= SEPARATE_RANGE * SEPARATE_RANGE)
                {
                    separateSum += -(distance / distanceSquared);
                    separateCount++;
                    
                    if (distanceSquared <= ALIGN_RANGE * ALIGN_RANGE)
                    {
                        alignSum += mVel.readCurrent(j);
                        alignCount++;
                        
                        if (distanceSquared <= GROUP_RANGE * GROUP_RANGE)
                        {
                            groupSum += mPos.readCurrent(j);
                            groupCount++;
                        }
                    }
                }
            }
        }
    }

    Vec2F force{};
    if (separateCount)
    {
        force += separateSum / separateCount * BOID_WEIGHTS.separate;
        
        if (alignCount)
        {
            force += (alignSum / alignCount - vel) * BOID_WEIGHTS.align;
            
            if (groupCount)
            {
                force += (groupSum / groupCount - pos).normalized() * BOID_WEIGHTS.group;
            }
        }
    }
    
    mVel.writeNext(index, vel + force * GetFrameTime());
    mDensity[index] = density;
}

void BoidManager::resolveTerrain(const size_t index)
{
    const Vec2I gridPos = mTerrain->getGridFromPos(mPos.readCurrent(index));
    
    Vec2F wallAvoid = Vec2F::Zero;
    float ttWeight = 0.0f;
    
    for (int x = -WALL_SEARCH_LENGTH; x <= WALL_SEARCH_LENGTH; x++)
    {			
        for (int y = -WALL_SEARCH_LENGTH; y <= WALL_SEARCH_LENGTH; y++)
        {
            if (!mTerrain->isWalkable(gridPos.x + x, gridPos.y + y))
            {
                float weight = 1.0f / (float)(x*x + y*y); // used to give less weight to further walls
                ttWeight += weight;
                wallAvoid += Vec2F((float)-x, (float)-y) * weight;
            }
        }
    }
    
    Vec2F addedVelocity = Vec2F::Zero;
    
    if (wallAvoid != Vec2F::Zero) addedVelocity += wallAvoid * WALL_WEIGHT;
    addedVelocity += mTerrain->getDirection(gridPos) * FLOW_FIELD_WEIGHT;
    
    mVel.writeNext(index, mVel.readNext(index) + addedVelocity * TERRAIN_WEIGHT);
}

void BoidManager::updateBoids()
{
    if (mBoidNumber == 0) return;
    
    if constexpr (USE_MULTITHREADING)
    {
        std::vector<std::thread> workers;
        const size_t chunkSize = (mBoidNumber + mThreadNumber - 1) / mThreadNumber;
    
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
                    if (mTerrain) resolveTerrain(i); // write all vel
                    integrate(i); // write all vel and pos
                }
            });
        }
        for (auto& w : workers) w.join();   // sync point
        workers.clear();
    }
    else
    {
        for (size_t i = 0; i < mBoidNumber; i++)
        {
            applyRules(i); // write all vel and density
            if (mTerrain) resolveTerrain(i); // write all vel
            integrate(i); // write all vel and pos
        }
    }

    mPos.swap();
    mVel.swap();
    
    mMaxDensity = 0;
    for (size_t index = 0; index < mBoidNumber; index++) mMaxDensity = std::max(mDensity[index], mMaxDensity);
}

void BoidManager::drawDebug(const size_t boidIndex) const
{
    DrawLineEx(mPos.readCurrent(boidIndex).toRaylib(), (mPos.readCurrent(boidIndex) + mVel.readCurrent(boidIndex) * 1).toRaylib(), 1.0f, GREEN);

    const float perceptionEdgeR = mVel.readCurrent(boidIndex).getRot() - PERCEPTION_ANGLE / 2;
    const float perceptionEdgeL = mVel.readCurrent(boidIndex).getRot() + PERCEPTION_ANGLE / 2;
    
    DrawCircleSectorLines(mPos.readCurrent(boidIndex).toRaylib(), GROUP_RANGE, perceptionEdgeR, perceptionEdgeL, 10, PINK);
    DrawCircleSectorLines(mPos.readCurrent(boidIndex).toRaylib(), ALIGN_RANGE, perceptionEdgeR, perceptionEdgeL, 10, ORANGE);
    DrawCircleSectorLines(mPos.readCurrent(boidIndex).toRaylib(), SEPARATE_RANGE, perceptionEdgeR, perceptionEdgeL, 10, RED);
    
    const Vec2I gridPos = mGrid.getGridPos(mPos.readCurrent(boidIndex));

    const int minX = std::max(0, gridPos.x - mCellStepX);
    const int maxX = std::min(mGrid.getGridSize().x, gridPos.x + mCellStepX + 1);
    const int minY = std::max(0, gridPos.y - mCellStepY);
    const int maxY = std::min(mGrid.getGridSize().y, gridPos.y + mCellStepY + 1);
    
    for (int y = minY; y < maxY; y++)
    {
        for (int x = minX; x < maxX; x++)
        {
            for (const uint32_t j : mGrid.getNeighbors(x,y))
            {
                if (boidIndex == j) continue;
                DrawCircleLines((int)mPos.readCurrent(j).x, (int)mPos.readCurrent(j).y, 20.0f, BLACK);
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
    mThreadNumber = Math::max(std::thread::hardware_concurrency(), 1u) * 2;
    
    const Vec2I gridSize = Vec2I::One + (mScreenBounds / GROUP_RANGE).to<int>();
    mGrid.resize(gridSize);
    
    mCellStepX = (int)std::ceil(GROUP_RANGE / mGrid.getCellSize().x / 2.0f);
    mCellStepY = (int)std::ceil(GROUP_RANGE / mGrid.getCellSize().y / 2.0f);
    
    mPos.resize(mBoidNumber, Vec2F::Zero);
    mVel.resize(mBoidNumber, Vec2F::Zero);
    mDensity.resize(mBoidNumber, 0);
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        mPos.writeNext(i, { Math::randFloat(0, mScreenBounds.x), Math::randFloat(0, mScreenBounds.y) });
        mVel.writeNext(i, randVec2() * MAX_SPEED);
    }
    
    mPos.copy();
    mVel.copy();
    
    mGrid.updateGrid(mPos.readVector());
    
    if (DEBUG_DENSITY)
    {
        for (uint32_t i = 0; i < mDensityPalette.size(); i++)
        {
            mDensityPalette[i] = colorLerp(RAYWHITE, RED, (float)i/(float)mDensityPalette.size());
        }
    }
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
    
    if constexpr (DEBUG_DENSITY) mGrid.draw();
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        Color color;
        if constexpr (DEBUG_DENSITY)
        {
            const int densityColor = (int)((float)mDensity[i]/(float)mMaxDensity * (float)mDensityPalette.size());
            color = mDensityPalette[densityColor];
        }
        else
        {
            color = BLUE;
        }
        
        const Vec2F p = mPos.readCurrent(i);
        Vec2F d = mVel.readCurrent(i).normalized();
        const Vec2F n(-d.y, d.x); // perpendicular

        const Vec2F tip   = p + d * BOID_SIZE;
        const Vec2F left  = p - d * (BOID_SIZE * 0.5f) + n * (BOID_SIZE * 0.5f);
        const Vec2F right = p - d * (BOID_SIZE * 0.5f) - n * (BOID_SIZE * 0.5f);

        DrawTriangle(right.toRaylib(), left.toRaylib(), tip.toRaylib(), color);
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
