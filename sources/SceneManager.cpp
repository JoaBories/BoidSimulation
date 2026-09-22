#include "SceneManager.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (!instance)
	{
		instance = new SceneManager();
	}

	return instance;
}

void SceneManager::ChangeScene(Scenes newScene)
{
	GameActor::Killa();

	mCurrentScene = newScene;

	switch (newScene)
	{
	case SceneDefault:
		break;

	default:
		break;
	}
}

void SceneManager::Update()
{
	switch (mCurrentScene)
	{
	case SceneDefault:
		break;

	default:
		break;
	}
}
