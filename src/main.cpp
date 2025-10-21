#include "raylib.h"
#include "Game.h"

int main() {
    // int display = GetCurrentMonitor();
    
    int screenWidth = 800;
    int screenHeight = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Go Game");

    SetTargetFPS(60);

    Game game(19, 19);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);

        game.Draw();

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}