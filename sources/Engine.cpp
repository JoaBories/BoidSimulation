#include "Engine.h"
#include "GameActor.h"

void Engine::init()
{
	InitWindow(800, 800, "Boid Simulation");
	SetTargetFPS(60);
	
	mAssetBank = AssetBank::GetInstance();
	mAssetBank->Init();
	
	time = 0.0f;
	
	mBoidManager = new BoidManager(0);

	mTerrain = new Terrain("resources/breeze_2048.png");
	
	Vec2I terrainSize = mTerrain->getSize();
	for (int i = 0; i < 100; i++)
	{
		Vec2I randomPos;
		do {
			randomPos = Vec2I(Math::randInt(0, terrainSize.x), Math::randInt(0, terrainSize.y));
		} while (!mTerrain->isWalkable(randomPos));
		
		mTerrain->newDestination(randomPos);		
	}
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
	
	time += GetFrameTime();
	if (time >= 60.0f)
	{
		mBoidManager->logAverageUpdate();
		time = 0.0f;
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
