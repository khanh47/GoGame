#pragma once
#include <string>

class HUD {
private:
    int _scorePlayer1;
    int _scorePlayer2;
    int _currentPlayer;

public:
    HUD();
    void render();
    void getScores(int scorePlayer1, int scorePlayer2, int currentPlayer);
};