#pragma once

#include "Util.h"
using Struct::Vect2F;

class GlobalVariables
{

public:
	//Engine Use ===============
	static bool EngineRunning; // engine has passed init state

	//Parameters ===============
	//Culling
	static bool TextureCulling; // Enable culling of invisible textures | Quite heavy enable it only for very huge texture
	static float CullingSizeMultiplier; // Scale the size of the CamSpace to avoid big texture false culling

	static bool ShowFPS; //Show FPS on screen
};

