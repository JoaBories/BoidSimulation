#pragma once
#include "GameActor.h"

class BoidManager
{
private:
    std::vector<Vect2F> mBoidPositions;
    std::vector<Vect2F> mBoidVelocities;
    uint32_t mBoidNumber;
    
public:
    BoidManager() = delete;
    explicit BoidManager(uint32_t agentNumber);
    
    ~BoidManager();
    
    void init();
    void update();
    void draw() const;
};
