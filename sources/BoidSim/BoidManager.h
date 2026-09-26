#pragma once
#include "BoidSim/BoidGrid.h"

struct BoidWeights
{
    float separate;
    float align;
    float group;
};

constexpr BoidWeights BOID_WEIGHTS = {10.0f, 1.0f, 0.5f};
constexpr float SEPARATE_RANGE = 30.0f;
constexpr float ALIGN_RANGE = 50.0f;
constexpr float GROUP_RANGE = 80.0f;
constexpr float PERCEPTION_ANGLE = 270.0f;
constexpr float MAX_SPEED = 50.0f;

class BoidManager
{
private:
    std::vector<Vec2F> mBoidPositions;
    std::vector<Vec2F> mBoidVelocities;
    uint32_t mBoidNumber;

    BoidGrid mGrid;
    
    uint64_t mLastUpdateTime;
    uint64_t mLastGridUpdateTime;
    uint64_t mUpdateCount;
    
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
