#pragma once

#include "Util.h"
using Struct::Vec2F;

class GlobalVariables
{
public:
	//Parameters ===============
	//Culling
	static bool TextureCulling; // Enable culling of invisible textures | Quite heavy enable it only for very huge texture
	static float CullingSizeMultiplier; // Scale the size of the CamSpace to avoid big texture false culling
};

