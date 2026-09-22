#pragma once
#include "GameActor.h"

class BoidManager
{
private:
    uint32_t mBoidNumber;
    std::vector<Vect2F> mBoidPositions;
    std::vector<Vect2F> mBoidVelocities;
    
public:
    BoidManager() = delete;
    BoidManager(uint32_t agentNumber);
    
    void init();
    void update();
    void draw();
};
