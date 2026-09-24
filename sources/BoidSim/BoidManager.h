#pragma once
#include "BoidSim/BoidGrid.h"

struct BoidWeights
{
    float separate;
    float align;
    float group;
};

class BoidManager
{
private:
    std::vector<Vec2F> mBoidPositions;
    std::vector<Vec2F> mBoidVelocities;
    std::vector<BoidWeights> mBoidWeights;
    uint32_t mBoidNumber;

    BoidGrid mGrid;
    
    uint64_t mLastUpdateTime;
    uint64_t mLastGridUpdateTime;
    uint64_t mUpdateCount;
    
    BoidWeights mDefaultWeights = {10.0f, 0.5f, 2.0f};
    float mSeparateRange = 40.0f;
    float mAlignRange = 50.0f;
    float mGroupRange = 70.0f;
    float mPerceptionAngle = 180.0f;
    float mMaxSpeed = 50.0f;
    
    void resolveVelocity();
    void checkScreenBounds();
    
    void applyRules();
    
    void drawDebug(uint32_t boidIndex) const;
    
public:
    BoidManager() = delete;
    explicit BoidManager(uint32_t agentNumber);
    
    ~BoidManager();
    
    BoidManager(const BoidManager& other) = delete;
    BoidManager(BoidManager&& other) noexcept = delete;
    BoidManager& operator=(const BoidManager& other) = delete;
    BoidManager& operator=(BoidManager&& other) noexcept = delete;
    
    void init();
    void update();
    void draw() const;
    
    void logAverageUpdate();
};
