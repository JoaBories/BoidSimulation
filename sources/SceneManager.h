#pragma once

// Child of GameActor
#include "ImageActor.h"
#include "TextActor.h"

enum Scenes
{
	SceneDefault,
};


class SceneManager
{
private:
	Scenes mCurrentScene;
	static SceneManager* instance;

public:
	SceneManager() = default;
	~SceneManager() = default;

	static SceneManager* GetInstance();

	void ChangeScene(Scenes newScene);
	inline Scenes GetCurrentScene() const { return mCurrentScene; };
	void Update();
};