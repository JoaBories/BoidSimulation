#pragma once
#include "GameActor.h"

class BoidManager : public GameActor
{
private:
    uint32_t mBoidNumber;
    std::vector<Vect2F> mBoidPositions;
    std::vector<Vect2F> mBoidVelocities;
    
public:
    BoidManager() = delete;
    BoidManager(uint32_t agentNumber);
    
    void Init() override;
    void Update() override;
    void Draw() override;
};
