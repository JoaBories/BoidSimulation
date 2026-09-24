#pragma once

#include "GlobalVariables.h"
using Struct::Rect2;

class Cam2D
{

private:
	Vec2F mPosition;
	float mZoom;

	static Cam2D* instance;

public:
	inline Cam2D() : mPosition{ Vec2F::Zero }, mZoom{ 1 } {}
	~Cam2D() = default;

	void Update();

	inline Vec2F GetPosition() const	{ return mPosition; }
	inline float GetZoom() const		{ return mZoom; }

	inline void SetPosition(Vec2F pos) { mPosition = pos; }

	Rect2 GetCamSpace() const;		//Rect2 of what the camera sees
	Rect2 GetSafeCamSpace() const;  //More wider rect for occlusion

	inline static Cam2D* GetInstance() {
		if (!instance) instance = new Cam2D(); return instance; }
};

