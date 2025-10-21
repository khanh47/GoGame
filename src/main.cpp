#include "raylib.h"
#include "Game.h"

int main() {
    // int display = GetCurrentMonitor();
    
    int screenWidth = 1200;
    int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Go Game");

    SetTargetFPS(60);

    Game game(19, 19);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);

        game.Draw();
        game.HandleInput();
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}