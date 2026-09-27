#include "Engine.h"
#include "GameActor.h"

void Engine::init()
{
	InitWindow(800, 800, "Boid Simulation");
	SetTargetFPS(60);
	
	mAssetBank = AssetBank::GetInstance();
	mAssetBank->Init();
	
	mBoidManager = new BoidManager(0);

	
	mTerrain = new Terrain("resources/breeze_1024.png");
	mTerrain->bench(1000);
}

void Engine::close()
{
	GameActor::killa();
	
	delete mAssetBank;
	mAssetBank = nullptr;
	
	delete mBoidManager;
	mBoidManager = nullptr;
}

void Engine::update()
{
	mBoidManager->update();
	mTerrain->update();
	
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
	
	mTerrain->draw();
	mBoidManager->draw();
	
	if (!GameActor::actors().empty())
	{
		for (GameActor* actor : GameActor::actors())
		{
			if (actor->IsActive()) actor->Draw();
		}
	}

	DrawFPS(10, 10);
	
	EndDrawing();
}
