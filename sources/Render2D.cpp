#include "Render2D.h"

Rect2 Render2D::toScreenSpace(Rect2 actorSpaceRect) const
{
	Rect2 textureToScreen = Rect2();

	Cam2D* camPtr = Cam2D::GetInstance();

	textureToScreen.center = (actorSpaceRect.center - camPtr->GetPosition()) * camPtr->GetZoom() + Vect2F{(float)GetScreenWidth(), (float)GetScreenHeight()} * 0.5f;
	textureToScreen.halfSize = actorSpaceRect.halfSize * camPtr->GetZoom();
	textureToScreen.rotation = actorSpaceRect.rotation;

	return textureToScreen;
}

bool Render2D::isVisible(Rect2 screenSpaceRect) const
{
	if (!GlobalVariables::TextureCulling)
	{
		return true;
	}

	std::vector<Vect2F> corners = screenSpaceRect.getCorners();

	Rect2 camSpace = Cam2D::GetInstance()->GetSafeCamSpace();

	for (const auto& corner : corners)
	{
		if (camSpace.ContainPoint(corner))
		{
			return true;
		}
	}

	return false;
}

Render2D::Render2D(Rect2 textureSpace):
	mTexture{ nullptr },
	mTextureSpace{ textureSpace }
{
}

void Render2D::Init(std::string textureName)
{
	mTexture = AssetBank::GetInstance()->GetATexture(textureName);
}

void Render2D::ChangeTexture(std::string textureName)
{
	mTexture = AssetBank::GetInstance()->GetATexture(textureName);
}

void Render2D::ChangeTextureSpace(Rect2 textureSpace)
{
	mTextureSpace = textureSpace;
}

Rect2 Render2D::GetTextureSpace() const
{
	return mTextureSpace;
}

bool Render2D::ShouldBeDrawn(const Transform2D& gameActorTransform) const
{
	Rect2 actorSpace = mTextureSpace.toObjectSpace(gameActorTransform);
	Rect2 screenSpace = toScreenSpace(actorSpace);

	return isVisible(screenSpace);
}

void Render2D::Draw(const Transform2D& gameActorTransform) const
{
	Rect2 actorSpace = mTextureSpace.toObjectSpace(gameActorTransform);
	Rect2 screenSpace = toScreenSpace(actorSpace);

	if (isVisible(screenSpace))
	{
		Texture text = *mTexture->pTexture;
		Rectangle sourceRect = { 0,0, (float)text.width, (float)text.height };
		Rectangle destRect = { screenSpace.center.x, screenSpace.center.y, screenSpace.halfSize.x * 2.0f, screenSpace.halfSize.y * 2.0f }; //actorSpace.toRaylib();
		
		DrawTexturePro(text, sourceRect, destRect, screenSpace.halfSize.toRaylib(), screenSpace.rotation, WHITE);
	}
}
