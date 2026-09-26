#pragma once

#include "Cam2D.h"
#include "AssetBank.h"

class Render2D
{
private:
	TextureEntry* mTexture;

	Rect2 mTextureSpace; // offset center, texture size, offset rotation

	Rect2 toScreenSpace(const Rect2& actorSpaceRect) const;

public:
	Render2D() = default;
	~Render2D() = default;

	Render2D(const Rect2& textureSpace);

	void Init(const std::string& textureName);

	void ChangeTexture(const std::string& textureName);
	void ChangeTextureSpace(const Rect2& textureSpace);

	Rect2 GetTextureSpace() const;

	bool ShouldBeDrawn(const Transform2D& gameActorTransform) const;
	void Draw(const Transform2D& gameActorTransform) const;
};

