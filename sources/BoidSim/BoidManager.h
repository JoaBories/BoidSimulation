#pragma once
#include "BoidSim/BoidGrid.h"

struct BoidWeights
{
    float separate;
    float align;
    float group;
};

constexpr BoidWeights BOID_WEIGHTS = {15.0f, 1.0f, 1.0f};
constexpr float SEPARATE_RANGE = 30.0f;
constexpr float ALIGN_RANGE = 50.0f;
constexpr float GROUP_RANGE = 80.0f;
constexpr float PERCEPTION_ANGLE = 270.0f;
constexpr float MAX_SPEED = 50.0f;

constexpr float DOT_PRODUCT_THRESHOLD = -(PERCEPTION_ANGLE / 180.0f - 1.0f);
constexpr float HIGHER_RANGE = Math::max(SEPARATE_RANGE, Math::max(ALIGN_RANGE, GROUP_RANGE));

class BoidManager
{
private:
    std::vector<Vec2F> mBoidPositions;
    std::vector<Vec2F> mBoidVelocities;
    uint32_t mBoidNumber;

    BoidGrid mGrid;
    
    uint64_t mTotalUpdateTime;
    uint64_t mUpdateCount;
    
    Vec2F mScreenBounds;
    
    void resolveVelocity(uint32_t boidIndex);
    void checkScreenBounds(uint32_t boidIndex);
    void applyRules(uint32_t boidIndex);
    void updateBoids();
    
    void drawDebug(uint32_t boidIndex) const;
    
public:
    BoidManager() = delete;
    explicit BoidManager(uint32_t agentNumber);
    
    ~BoidManager();
    
    BoidManager(const BoidManager& other) = delete;
    BoidManager(BoidManager&& other) noexcept = delete;
    BoidManager& operator=(const BoidManager& other) = delete;
    BoidManager& operator=(BoidManager&& other) noexcept = delete;
    
    void update();
    void draw() const;
    
    void logAverageUpdate() const;
};
