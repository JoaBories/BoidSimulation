#include "Engine.h"

#include "BoidSim/BoidScene.h"

void Engine::Init()
{
	mAssetBank = AssetBank::GetInstance();
	mCamera = Cam2D::GetInstance();

	mAssetBank->Init();
	mCamera->SetPosition({ 0.0f, 0.0f});

	InitActors(); // for actors created before (if there is)

	GlobalVariables::EngineRunning = true;

	SceneManager::changeScene(new BoidScene());
}

void Engine::InitActors()
{
	if (GameActor::GetActorsLogic().empty())
	{
		return;
	}

	for (auto& actorList : GameActor::GetActorsLogic())
	{
		for (auto& actor : actorList.second)
		{
			actor->Init();
		}
	}
}

void Engine::DeInit()
{
	GameActor::Killa();
	
	delete mAssetBank;
	delete mCamera;
}

void Engine::Update()
{
	mCamera->Update();

	UpdateActors();

	GameActor::KillPendingsActors();
}

void Engine::UpdateActors()
{
	if (GameActor::GetActorsLogic().empty())
	{
		return;
	}

	for (auto& actorList : GameActor::GetActorsLogic())
	{
		for (auto& actor : actorList.second)
		{
			if (actor->IsActive())
			{
				actor->Update();
			}
		}
	}
}

void Engine::Draw()
{
	DrawActors();

	if (GlobalVariables::ShowFPS)
	{
		DrawFPS(50, 50);
	}
}

void Engine::DrawActors()
{
	if (GameActor::GetActorsRender().empty())
	{
		return;
	}

	for (auto& actorList : GameActor::GetActorsRender())
	{
		for (auto& actor : actorList.second)
		{
			if (actor->IsActive())
			{
				actor->Draw();
			}
		}
	}
}
