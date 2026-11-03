#include "Engine.h"
#include "GameActor.h"

void Engine::init()
{
	SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_MSAA_4X_HINT);
	InitWindow(1920, 1080, "Boid Simulation");
	
	SetTargetFPS(60);
	
	mAssetBank = AssetBank::GetInstance();
	mAssetBank->Init();
	
	mBoidManager = new BoidManager(7500);
	//mTerrain = new Terrain("resources/breeze_1024.png");
	
	if (mBoidManager && mTerrain) mBoidManager->setTerrain(mTerrain);
}

void Engine::close()
{
	GameActor::killa();
	
	delete mAssetBank;
	mAssetBank = nullptr;
	
	if (mBoidManager) delete mBoidManager;
	mBoidManager = nullptr;
	
	if (mTerrain) delete mTerrain;
	mTerrain = nullptr;
}

void Engine::update()
{
	if (mBoidManager) mBoidManager->update();
	if (mTerrain) mTerrain->update();
	
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
	
	if (mTerrain) mTerrain->draw();
	if (mBoidManager) mBoidManager->draw();
	
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
