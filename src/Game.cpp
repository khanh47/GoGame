#include "Game.h"
#include "raylib.h"
#include "Board.h"
#include "colors.h"
#include <cmath>

Game::Game(int rows, int cols) : grid(std::make_unique<Board>(rows, cols))
{
}

Game::~Game()
{
}

void Game::render()
{
    grid->render();
}

void Game::handleInput()
{
    if (_isGameOver) return;
    Vector2 mousePos = GetMousePosition();
    int row = (int)round((mousePos.x - CELL_SIZE - PADDING) / CELL_SIZE);
    int col = (int)round((mousePos.y - CELL_SIZE - PADDING) / CELL_SIZE);
    if (grid->isCellInside(row, col) && grid->getValue(row, col) == 0)
    {
        grid->renderGhostStones(row, col, _currentPlayer);
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (grid->isCellInside(row, col) && grid->getValue(row, col) == 0)
        {
            grid->setValue(row, col, _currentPlayer);
            _currentPlayer = _currentPlayer == 1 ? 2 : 1;
        }
    }
}
