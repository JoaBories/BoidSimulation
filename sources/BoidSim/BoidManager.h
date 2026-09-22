#pragma once
#include "GameActor.h"

struct BoidWeights
{
    float separate;
    float align;
    float group;
};

class BoidManager
{
private:
    std::vector<Vect2F> mBoidPositions;
    std::vector<Vect2F> mBoidVelocities;
    std::vector<BoidWeights> mBoidWeights;
    uint32_t mBoidNumber;
    
    int mLastUpdateTime;
    
    BoidWeights mDefaultWeights = {5.0f, 0.5f, 2.0f};
    float mSeparateRange = 20.0f;
    float mAlignRange = 50.0f;
    float mGroupRange = 70.0f;
    float mPerceptionAngle = 180.0f;
    float mMaxSpeed = 100.0f;
    
    void resolveVelocity();
    void checkScreenBounds();
    
    void applyRules();
    
    void drawDebug(uint32_t boidIndex) const;
    
public:
    BoidManager() = delete;
    explicit BoidManager(uint32_t agentNumber);
    
    ~BoidManager();
    
    void init();
    void update();
    void draw() const;
};
