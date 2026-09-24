#include "ImageActor.h"

ImageActor::ImageActor(const Vec2F position, const Vec2F size, const std::string& texture) :
	GameActor{2,0, {position, Vec2F::One, 0}, TagDefault},
	mRender{Rect2{Vec2F::Zero, size, 0.0f}}
{
	ImageActor::Init();
	mRender.Init(texture);
}

void ImageActor::Init()
{
}

void ImageActor::Update()
{
}

void ImageActor::Draw()
{
	mRender.Draw(mTransform);
}
