#include "Cam2D.h"

Cam2D* Cam2D::instance = nullptr;

void Cam2D::Update()
{
    //Movement if you want
}

Rect2 Cam2D::GetCamSpace() const
{
    Vect2F halfSize = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    Vect2F center = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    Rect2 camSpace = { center * 0.5f, halfSize, 0};

    return camSpace;
}

Rect2 Cam2D::GetSafeCamSpace() const
{
    Rect2 safeCamSpace = GetCamSpace();
    safeCamSpace.halfSize *= GlobalVariables::CullingSizeMultiplier;

    return safeCamSpace;
}
