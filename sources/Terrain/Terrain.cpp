#include "Terrain.h"

using Struct::Rect2;

Terrain::Terrain(std::string imagePath)
{
	Image map = LoadImage(imagePath.c_str());
	mMapTexture = LoadTextureFromImage(map);
	UnloadImage(map);
}

void Terrain::draw() const 
{
	Vec2F screenSize{ (float)GetScreenWidth(), (float)GetScreenHeight() };

	Rectangle sourceRect = { 0,0, (float)mMapTexture.width, (float)mMapTexture.height };
	Rectangle destRect = { 0, 0, GetScreenWidth(), GetScreenHeight()};

	DrawTexturePro(mMapTexture, sourceRect, destRect, {0,0}, 0, WHITE);
}
