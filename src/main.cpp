#include "raylib.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameState.h"

int main() {
    int screenWidth = 1200;
    int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Go Game");
    SetTargetFPS(60);

    ResourceManager &resourceManager = ResourceManager::getInstance();
    GameStateModel gamestate;
    SceneManager sceneManager(&gamestate);

    while (!WindowShouldClose()) {
        BeginDrawing();

        sceneManager.update(GetFrameTime());
        sceneManager.render();
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}