#include "Engine.h"

void Engine::Init()
{
	mSceneManager = SceneManager::GetInstance();
	mAssetBank = AssetBank::GetInstance();
	mCamera = Cam2D::GetInstance();

	mAssetBank->Init();
	mCamera->SetPosition({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 });

	InitActors(); // for actors created before (if there is)

	GlobalVariables::EngineRunning = true;

	mSceneManager->ChangeScene(SceneDefault);

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

	delete mSceneManager;
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
		DrawFPS(100, 100);
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
