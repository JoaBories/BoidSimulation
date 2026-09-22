#include "Engine.h"

void Init();
void DeInit();
void UpdateDraw();
void Update();
void Draw();

Engine mEngine;

int screenHeight = 600; // Window Size
int screenWidth = 700;  //

Color backgroundColor = BLACK;

const char* gameName = "Starter Engine"; //Name of the window

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
    srand((unsigned int) time(0));

    InitWindow(screenWidth, screenHeight, gameName);
    SetTargetFPS(60);

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

