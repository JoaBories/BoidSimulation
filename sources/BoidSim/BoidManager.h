#pragma once
#include <thread>

#include "DoubleBufferVector.h"
#include "BoidSim/BoidGrid.h"
#include "Terrain/Terrain.h"
#include <array>

struct BoidWeights
{
    float separate;
    float align;
    float group;
};

constexpr BoidWeights BOID_WEIGHTS = {25.0f, 0.5f, 0.6f};
constexpr float TERRAIN_WEIGHT = 1.0f;
constexpr float WALL_WEIGHT = 5.0f;
constexpr float FLOW_FIELD_WEIGHT = 0.9f;

constexpr float SEPARATE_RANGE = 30.0f;
constexpr float ALIGN_RANGE = 45.0f;
constexpr float GROUP_RANGE = 60.0f;
constexpr float PERCEPTION_ANGLE = 360.0f;

constexpr float MAX_SPEED = 20.0f;
constexpr float MIN_SPEED = 15.0f;

constexpr float BOID_SIZE = 2.0f;

constexpr int WALL_SEARCH_LENGTH = 4; // in pixel on map

constexpr bool DEBUG_PERF = true;
constexpr bool DEBUG_DENSITY = true;

constexpr bool USE_MULTITHREADING = true;

constexpr float DOT_PRODUCT_THRESHOLD = -(PERCEPTION_ANGLE / 180.0f - 1.0f);

class BoidManager
{
private:
    DoubleBufferVector<Vec2F> mPos;
    DoubleBufferVector<Vec2F> mVel;
    std::vector<float> mDensity;
    
    std::array<Color, 32> mDensityPalette;
    
    uint64_t mBoidNumber;

    BoidGrid mGrid;
    int mThreadNumber;
    
    int mCellStepX;
    int mCellStepY;
    
    Vec2F mScreenBounds;
    
    uint64_t mTotalUpdateTime;
    uint64_t mUpdateCount;
    float mLogTime;
    
    float mMaxDensity;
    
    Terrain* mTerrain;
    
    void integrate(size_t index);
    void applyRules(size_t index);
    void resolveTerrain(size_t index);
    void updateBoids();
    
    void drawDebug(size_t boidIndex) const;
    
public:
    BoidManager() = delete;
    explicit BoidManager(size_t agentNumber);
    ~BoidManager() = default;
    
    BoidManager(const BoidManager& other) = delete;
    BoidManager(BoidManager&& other) noexcept = delete;
    BoidManager& operator=(const BoidManager& other) = delete;
    BoidManager& operator=(BoidManager&& other) noexcept = delete;
    
    void update();
    void draw() const;
    
    void setTerrain(Terrain* terrain) { mTerrain = terrain; }
    
    void logAverageUpdate() const;
};
