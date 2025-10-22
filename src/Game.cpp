#include "Game.h"
#include "raylib.h"
#include "Board.h"
#include "components/colors.h"
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

void Game::render()
{
    grid.render();
}

void Game::HandleInput()
{
    if (IsGameOver)
        return;
    Vector2 mousePos = GetMousePosition();
    int row = (int)round((mousePos.x - CELL_SIZE - PADDING) / CELL_SIZE);
    int col = (int)round((mousePos.y - CELL_SIZE - PADDING) / CELL_SIZE);
    if (grid.isCellInside(row, col) && grid.getValue(row, col) == 0)
    {
        grid.renderGhostStones(row, col, CurrentPlayer);
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (grid.isCellInside(row, col) && grid.getValue(row, col) == 0)
        {
            grid.setValue(row, col, CurrentPlayer);
            CurrentPlayer = CurrentPlayer == 1 ? 2 : 1;
        }
    }
}
