#include "Engine.h"
#include "GameActor.h"

void Engine::init()
{
	SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	InitWindow(800, 800, "Boid Simulation");
	
	SetTargetFPS(60);
	
	mAssetBank = AssetBank::GetInstance();
	mAssetBank->Init();
	
	mBoidManager = new BoidManager(1000);
	mTerrain = new Terrain("resources/breeze_1024.png");
}

void Engine::close()
{
	GameActor::killa();
	
	delete mAssetBank;
	mAssetBank = nullptr;
	
	delete mBoidManager;
	mBoidManager = nullptr;
	
	delete mTerrain;
	mTerrain = nullptr;
}

void Engine::update()
{
	mBoidManager->update();
	//mTerrain->update();
	
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
	ClearBackground({ 20, 20, 20, 255 });
	
	//mTerrain->draw();
	mBoidManager->draw();
	
	if (!GameActor::actors().empty())
	{
		for (GameActor* actor : GameActor::actors())
		{
			if (actor->IsActive()) actor->Draw();
		}
	}

	if constexpr (DEBUG_PERF) DrawFPS(10, 10);
	
	EndDrawing();
}
