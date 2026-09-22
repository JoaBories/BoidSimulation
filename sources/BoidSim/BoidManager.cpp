#include "BoidManager.h"

BoidManager::BoidManager(uint32_t agentNumber) :
    mBoidNumber(agentNumber)
{
    if (GlobalVariables::EngineRunning)
    {
        BoidManager::Init();
    }
}

void BoidManager::Init()
{
    mBoidPositions.reserve(mBoidNumber);
    mBoidVelocities.reserve(mBoidNumber);
    
    const Vect2F screenCenter = {static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f};
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        mBoidPositions.emplace_back(screenCenter);
        mBoidVelocities.emplace_back(MathUtils::RandVect2Normalized() * 50.0f);
    }
}

void BoidManager::Update()
{
    Vect2F screenBounds = Vect2F(static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()));
    
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        mBoidPositions[i] += mBoidVelocities[i] * GetFrameTime();
        
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

void BoidManager::Draw()
{
    for (uint32_t i = 0; i < mBoidNumber; i++)
    {
        const float rotation = mBoidVelocities[i].getRot();
        const Vect2F position = mBoidPositions[i];
        
        DrawCircleV(mBoidPositions[i].toRaylib(), 2.0f, DARKGRAY);
    }
}
