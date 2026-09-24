#include "Engine.h"

int main() {

    Engine* engine = new Engine();
    engine->init();
    
    while (!WindowShouldClose()) 
    {
        engine->update();
        engine->draw();
    }

    engine->close();
    delete engine;
    
    CloseWindow();
    
    return 0;
}