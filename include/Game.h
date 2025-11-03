#pragma once
#include <memory>

// Forward declaration
class Board;

class Game {
public:
    Game(int rows, int cols);
    ~Game();
    void render();
    void handleInput();
    int getCurrentPlayer() const { return _currentPlayer; }
    int getScorePlayer1() const { return _scorePlayer1; }
    int getScorePlayer2() const { return _scorePlayer2; }
    bool isGameOver() const { return _isGameOver; }

private:
    std::unique_ptr<Board> grid;
    bool _isGameOver = false;
    int _currentPlayer = 1; // 1 for black, 2 for white
    int _scorePlayer1 = 0;
    int _scorePlayer2 = 0;

};