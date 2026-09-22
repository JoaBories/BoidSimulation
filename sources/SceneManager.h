#pragma once

// Child of GameActor
#include "ImageActor.h"

enum Scenes
{
	SceneDefault,
};

class Scene
{
public:
	virtual ~Scene() = default;
	
	virtual void update() = 0;
	virtual void draw() = 0;
	
	virtual void init() = 0;
};


class SceneManager
{
private:
	static Scene* mCurrentScene;

public:
	SceneManager() = default;
	~SceneManager() = default;

	static void changeScene(Scene* newScene);
	static Scene* getCurrentScene() { return mCurrentScene; }
	
	static void update();
	static void draw();
};