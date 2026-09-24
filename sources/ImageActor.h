#pragma once
#include "GameActor.h"
#include "Render2D.h"

class ImageActor : public GameActor
{
private:
	Render2D mRender;

public:

	ImageActor() = default;
	~ImageActor() = default;

	ImageActor(Vec2F position, Vec2F size, const std::string& texture);

	// Inherited via GameActor
	void Init() override;
	void Update() override;
	void Draw() override;
};

