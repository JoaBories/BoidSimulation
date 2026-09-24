#pragma once

#include "BoidSim/BoidManager.h"
#include "AssetBank.h"

class Engine
{
private :
	AssetBank* mAssetBank;
	BoidManager* mBoidManager;
	
	float time;

public :
	Engine() = default;
	~Engine() = default;
	
	Engine(const Engine& other) = delete;
	Engine(Engine&& other) noexcept = delete;
	Engine& operator=(const Engine& other) = delete;
	Engine& operator=(Engine&& other) noexcept = delete;

	void init();
	void close();

	void update();
	void draw() const;
};

