#include "Engine.h"

void Init();
void DeInit();
void UpdateDraw();
void Update();
void Draw();

Engine mEngine;

int screenHeight = 800; // Window Size
int screenWidth = 800;  //

Color backgroundColor = RAYWHITE;

const char* gameName = "BoidSim"; //Name of the window

int main() {

    Init();
    
    while (!WindowShouldClose()) {
        
        UpdateDraw();
    }

    DeInit();
    
    return 0;
}

void Init()
{
    srand(static_cast<unsigned int>(time(0)));

    InitWindow(screenWidth, screenHeight, gameName);
    SetTargetFPS(0);

    mEngine.Init();
}

void DeInit()
{
    mEngine.DeInit();

    CloseWindow();
}

void UpdateDraw()
{
    Update();

    BeginDrawing();
    ClearBackground(backgroundColor);

    Draw();
    EndDrawing();
}

void Update()
{
    mEngine.Update();
}

void Draw()
{
    mEngine.Draw();
}

