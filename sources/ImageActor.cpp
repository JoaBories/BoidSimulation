#include "ImageActor.h"

ImageActor::ImageActor(Vect2F position, Vect2F size, std::string texture) :
	GameActor{2,0, {position, Vect2F::one, 0}, TagDefault},
	mRender{{Vect2F::zero, size}}
{
	if (GlobalVariables::EngineRunning)
	{
		Init();
		mRender.Init(texture);
	}
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
