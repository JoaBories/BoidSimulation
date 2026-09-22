#include "ImageActor.h"

ImageActor::ImageActor(const Vect2F position, const Vect2F size, const std::string& texture) :
	GameActor{2,0, {position, Vect2F::one, 0}, TagDefault},
	mRender{Rect2{Vect2F::zero, size, 0.0f}}
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
