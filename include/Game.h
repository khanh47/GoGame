#pragma once
#include "Board.h"

class Game {
public:
    Game(int rows, int cols);
    ~Game();
    void Draw();
    void HandleInput();
    bool IsGameOver;
    bool CurrentPlayer;
    int scorePlayer1;
    int scorePlayer2;
    
private:
    Board grid;
};