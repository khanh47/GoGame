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
    Vector2 mousePos = GetMousePosition();
    int row = (int)round((mousePos.x - CELL_SIZE - PADDING) / CELL_SIZE);
    int col = (int)round((mousePos.y - CELL_SIZE - PADDING) / CELL_SIZE);
    if (grid->isCellValid(row, col)) {
        grid->renderGhostStones(row, col, _currentPlayer);
    }
}

void Game::handleInput()
{
    if (_isGameOver) return;
    Vector2 mousePos = GetMousePosition();
    bool isMouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    int row = (int)round((mousePos.x - CELL_SIZE - PADDING) / CELL_SIZE);
    int col = (int)round((mousePos.y - CELL_SIZE - PADDING) / CELL_SIZE);
    
    if (isMouseClicked && grid->isCellValid(row, col)) {
        grid->setValue(row, col, _currentPlayer);
        _currentPlayer = _currentPlayer == 1 ? 2 : 1;
    }
}
