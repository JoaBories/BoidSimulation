#pragma once
#include "DoubleBufferVector.h"
#include "BoidSim/BoidGrid.h"

struct BoidWeights
{
    float separate;
    float align;
    float group;
};

constexpr BoidWeights BOID_WEIGHTS = {2.0f, 0.9f, 0.6f};
constexpr float SEPARATE_RANGE = 30.0f;
constexpr float ALIGN_RANGE = 50.0f;
constexpr float GROUP_RANGE = 70.0f;
constexpr float PERCEPTION_ANGLE = 120.0f;
constexpr float MAX_SPEED = 50.0f;

constexpr bool BOID_WRAP = false;
constexpr bool DEBUG_PERF = true;
constexpr bool DEBUG_DENSITY = true;

constexpr float DOT_PRODUCT_THRESHOLD = -(PERCEPTION_ANGLE / 180.0f - 1.0f);
constexpr float HIGHER_RANGE = Math::max(SEPARATE_RANGE, Math::max(ALIGN_RANGE, GROUP_RANGE));

class BoidManager
{
private:
    DoubleBufferVector<Vec2F> mPos;
    DoubleBufferVector<Vec2F> mVel;
    DoubleBufferVector<float> mDensity;
    uint32_t mBoidNumber;

    BoidGrid mGrid;
    Vec2I mCellStep;
    Vec2F mScreenBounds;
    
    uint64_t mTotalUpdateTime;
    uint64_t mUpdateCount;
    float mLogTime;
    
    float mMaxDensity;
    
    
    void resolveMovement(size_t index);
    void checkScreenBounds(size_t index);
    void applyRules(size_t index);
    void updateBoids();
    
    void drawDebug(uint32_t boidIndex) const;
    
public:
    BoidManager() = delete;
    explicit BoidManager(uint32_t agentNumber);
    ~BoidManager() = default;
    
    BoidManager(const BoidManager& other) = delete;
    BoidManager(BoidManager&& other) noexcept = delete;
    BoidManager& operator=(const BoidManager& other) = delete;
    BoidManager& operator=(BoidManager&& other) noexcept = delete;
    
    void update();
    void draw() const;
    
    void logAverageUpdate() const;
};
