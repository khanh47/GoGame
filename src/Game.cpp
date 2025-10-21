#include "Game.h"
#include "raylib.h"
#include "colors.h"
#include <iostream>
#include <cmath>
#include <iostream>
#include <cctype>

Game::Game(int rows, int cols) : grid(rows, cols)
{
    IsGameOver = false;
    CurrentPlayer = 1;
    scorePlayer1 = 0;
    scorePlayer2 = 0;
}

Game::~Game()
{
}

void Game::Draw()
{
    grid.Draw();
}

void Game::HandleInput()
{
    if (IsGameOver)
        return;
    Vector2 mousePos = GetMousePosition();
    int cellSize = 36;
    int padding = 36;
    float radius = cellSize * 0.4f;
    int row = (int)round((mousePos.x - cellSize - padding) / cellSize);
    int col = (int)round((mousePos.y - cellSize - padding) / cellSize);
    if (grid.IsCellInside(row, col) && grid.GetValue(row, col) == 0)
    {
        DrawCircle(padding + (row + 1) * cellSize, padding + (col + 1) * cellSize, radius, CurrentPlayer ? blackHue : whiteHue);
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (grid.IsCellInside(row, col) && grid.GetValue(row, col) == 0)
        {
            grid.SetValue(row, col, CurrentPlayer + 1);
            CurrentPlayer ^= 1;
        }
    }
}
