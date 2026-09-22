#include "BoidManager.h"

void BoidManager::resolveVelocity()
{
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        if (mBoidVelocities[i].sqrLength() > mMaxSpeed * mMaxSpeed) mBoidVelocities[i] = mBoidVelocities[i].normalized() * mMaxSpeed;
        mBoidPositions[i] += mBoidVelocities[i] * GetFrameTime();
    }
}

void BoidManager::checkScreenBounds()
{
    const Vect2F screenBounds(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()));
    
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
    const float dotProductTreshold = -(mPerceptionAngle / 180.0f - 1.0f);
    const float higherRange = Math::Max(mSeparateRange, Math::Max(mAlignRange, mGroupRange));
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        Vect2F separate { 0.0f, 0.0f };
        
        Vect2F alignSum { 0.0f, 0.0f };
        uint32_t alignCount = 0;
        
        Vect2F groupSum { 0.0f, 0.0f };
        uint32_t groupCount = 0;
        
        for (uint32_t j = 0; j < mBoidNumber; j++)
        {
            if (i == j) continue;
            
            Vect2F distance = mBoidPositions[j] - mBoidPositions[i];
            if (distance.dot(mBoidVelocities[i]) <= dotProductTreshold) // Skip neighbors not in view
            {
                continue;
            }

            const float distanceSquared = distance.dot(distance);
            if (distanceSquared >= higherRange * higherRange) // Skip neighbors not in range
            {
                continue;
            }
            
            if (distanceSquared <= mSeparateRange * mSeparateRange)
            {
                separate += -distance / distanceSquared;
            }
            
            if (distanceSquared <= mAlignRange * mAlignRange)
            {
                alignSum += mBoidVelocities[j];
                alignCount++;
            }
            
            if (distanceSquared <= mGroupRange * mGroupRange)
            {
                groupSum += mBoidPositions[j];
                groupCount++;
            }
        }
        
        separate = separate.normalized();
        
        Vect2F align { 0.0f, 0.0f };
        if (alignCount)
        {
            alignSum /= static_cast<float>(alignCount);
            align = alignSum.normalized() * mMaxSpeed;
        }
        
        Vect2F group { 0.0f, 0.0f };
        if (groupCount)
        {
            groupSum /= static_cast<float>(groupCount);
            group = (groupSum - mBoidPositions[i]).normalized();
        }
        
        Vect2F force = separate * mBoidWeights[i].separate + align * mBoidWeights[i].align + group * mBoidWeights[i].group;
        mBoidVelocities[i] += force * GetFrameTime();
    }
}

void BoidManager::drawDebug(const uint32_t boidIndex) const
{
    DrawLineEx(mBoidPositions[boidIndex].toRaylib(), (mBoidPositions[boidIndex] + mBoidVelocities[boidIndex] * 1).toRaylib(), 1.0f, GREEN);

    const float perceptionEdgeR = mBoidVelocities[boidIndex].getRot() - mPerceptionAngle / 2;
    const float perceptionEdgeL = mBoidVelocities[boidIndex].getRot() + mPerceptionAngle / 2;
    
    DrawCircleSectorLines(mBoidPositions[boidIndex].toRaylib(), mGroupRange, perceptionEdgeR, perceptionEdgeL, 10, PINK);
    DrawCircleSectorLines(mBoidPositions[boidIndex].toRaylib(), mAlignRange, perceptionEdgeR, perceptionEdgeL, 10, ORANGE);
    DrawCircleSectorLines(mBoidPositions[boidIndex].toRaylib(), mSeparateRange, perceptionEdgeR, perceptionEdgeL, 10, RED);
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
    const Vect2F screenBounds(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()));
    
    mBoidPositions.reserve(mBoidNumber);
    mBoidVelocities.reserve(mBoidNumber);
    mBoidWeights.reserve(mBoidNumber);
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        mBoidPositions.emplace_back(Math::RandFloat(0, screenBounds.x), Math::RandFloat(0, screenBounds.y));
        mBoidVelocities.emplace_back(Math::randVec2() * mMaxSpeed);
        mBoidWeights.emplace_back(mDefaultWeights);
    }
}

void BoidManager::update()
{
    checkScreenBounds();
    resolveVelocity();
    
    applyRules();
}

void BoidManager::draw() const
{
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        DrawCircleV(mBoidPositions[i].toRaylib(), 2.0f, DARKGRAY);
    }
    
    //draw debug
    drawDebug(0);
}
