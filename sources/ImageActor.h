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

	ImageActor(Vect2F position, Vect2F size, std::string texture);

	// Inherited via GameActor
	void Init() override;
	void Update() override;
	void Draw() override;
};

