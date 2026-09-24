#include "Engine.h"
#include "GameActor.h"

void Engine::init()
{
	InitWindow(800, 800, "Boid Simulation");
	SetTargetFPS(60);
	
	mAssetBank = AssetBank::GetInstance();
	mAssetBank->Init();
	
	time = 0.0f;
	
	mBoidManager = new BoidManager(500);
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
