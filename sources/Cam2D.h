#pragma once

#include "GlobalVariables.h"
using Struct::Rect2;

class Cam2D
{

private:
	Vect2F mPosition;
	float mZoom;

	static Cam2D* instance;

public:
	inline Cam2D() : mPosition{ Vect2F::zero }, mZoom{ 1 } {};
	~Cam2D() = default;

	void Update();

	inline Vect2F GetPosition() const	{ return mPosition; };
	inline float GetZoom() const		{ return mZoom; };

	inline void SetPosition(Vect2F pos) { mPosition = pos; };

	Rect2 GetCamSpace() const;		//Rect2 of what the camera sees
	Rect2 GetSafeCamSpace() const;  //More wider rect for occlusion

	inline static Cam2D* GetInstance() {
		if (!instance) instance = new Cam2D(); return instance; };
};

