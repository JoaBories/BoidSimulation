#include "Engine.h"

void Engine::init()
{
	InitWindow(800, 800, "Boid Simulation");
	SetTargetFPS(60);
	
	mAssetBank = AssetBank::GetInstance();
	mAssetBank->Init();
	
	mBoidManager = new BoidManager(10);
}

void Engine::close()
{
	GameActor::killa();
	
	delete mAssetBank;
	mAssetBank = nullptr;
	
	delete mBoidManager;
	mBoidManager = nullptr;
}

void Engine::update() const
{
	mBoidManager->update();
	
	if (!GameActor::actors().empty())
	{
		for (GameActor* actor : GameActor::actors())
		{
			if (actor->IsActive()) actor->Update();
		}
	}

	GameActor::killPendingsActors();
}

void Engine::draw() const
{
	BeginDrawing();
	ClearBackground(RAYWHITE);
	
	mBoidManager->draw();
	
	if (!GameActor::actors().empty())
	{
		for (GameActor* actor : GameActor::actors())
		{
			if (actor->IsActive()) actor->Draw();
		}
	}

	DrawFPS(50, 50);
	
	EndDrawing();
}
