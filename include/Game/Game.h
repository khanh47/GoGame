#pragma once
#include <vector>
#include <string>

// Forward declaration
class Board;

class Game {
public:
    Game(int rows, int cols, Board* board);
    ~Game();
    void render();
    bool handleInput();

		void resetGame();
		void passTurn();
    void DetermineTerritories();
    void CheckValid(int CurrentPlayer, int OpponentPlayer);
    void BeingCaptured(int CurrentPlayer, int OpponentPlayer, std::pair<int, int> lastMove);
    void SelfCapture(int CurrentPlayer, std::vector<std::pair<int, int>> Stones, std::vector<std::pair<int, int>> EmptySpots);

    void setCurrentPlayer(const int& currentPlayer) { _currentPlayer = currentPlayer; }
    void setScorePlayer1(const int& scorePlayer1) { _scorePlayer1 = scorePlayer1; }
    void setScorePlayer2(const int& scorePlayer2) { _scorePlayer2 = scorePlayer2; }

    int getCurrentPlayer() const { return _currentPlayer; }
    int getScorePlayer1() const { return _scorePlayer1; }
    int getScorePlayer2() const { return _scorePlayer2; }
		int getFinalScorePlayer1() const { return _FinalScorePlayer1; }
		int getFinalScorePlayer2() const { return _FinalScorePlayer2; }
    bool isGameOver() const { return _isGameOver; }
		std::string getGameMode() const { return _gameMode; }

		std::vector<std::vector<int>> getGrid();
		std::vector<std::vector<int>> getValidPlayer1Map();
		std::vector<std::vector<int>> getValidPlayer2Map();

private:
		Board* _board = nullptr;
		int _rows = 0;
		int _cols = 0;
		bool _isLastTurnPass = false;
    bool _isGameOver = false;
    int _currentPlayer = 1; // 1 for black, 2 for white
    int _scorePlayer1 = 0;
    int _scorePlayer2 = 0;
    int _FinalScorePlayer1 = 0;
    int _FinalScorePlayer2 = 0;
		std::string _gameMode = "NONE";
};
