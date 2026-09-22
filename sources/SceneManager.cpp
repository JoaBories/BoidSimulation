#include "SceneManager.h"

Scene* SceneManager::mCurrentScene = nullptr;

void SceneManager::changeScene(Scene* newScene)
{
	GameActor::Killa();
	mCurrentScene = newScene;
	mCurrentScene->init();
}

void SceneManager::update()
{
	if (mCurrentScene) mCurrentScene->update();
}

void SceneManager::draw()
{
	if (mCurrentScene) mCurrentScene->draw();
}
