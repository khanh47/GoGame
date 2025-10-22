#pragma once
#include "components/Render/Board.h"

class Game {
public:
    Game(int rows, int cols);
    ~Game();
    void render();
    void HandleInput();
    bool IsGameOver;
    int CurrentPlayer;
    int scorePlayer1;
    int scorePlayer2;
    
private:
    Board grid;
};