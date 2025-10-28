#pragma once
#include "components/Render/Board.h"

class Game {
public:
    Game(int rows, int cols);
    ~Game();
    void render();
    void HandleInput();
    bool IsGameOver;
    int CurrentPlayer; // 1 for black, 2 for white
    int scorePlayer1;
    int scorePlayer2;
    
private:
    Board grid;
};