#include "Game.h"

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
    grid.SetValue(3, 3, 1);
    grid.SetValue(6, 6, 2);
}

void Game::HandleInput()
{
    if (IsGameOver)
        return;
    
}
