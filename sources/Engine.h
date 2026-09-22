#pragma once

#include "SceneManager.h"

class Engine
{
private :

	SceneManager* mSceneManager;
	AssetBank* mAssetBank;
	Cam2D* mCamera;

public :
	Engine() = default;
	~Engine() = default;

	void Init();
	void InitActors();

	void DeInit();

	void Update();
	void UpdateActors();

	void Draw();
	void DrawActors();
};

