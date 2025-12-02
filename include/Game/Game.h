#pragma once
#include "GroupManager.h"
#include <string>
#include <vector>
#include <memory>

// Forward declaration
class Board;
class GoAI;

class Game {
private:
    // AI
    std::unique_ptr<GoAI> _ai;
    bool _isAIEnabled = false;

    int _aiColor = 2;  // AI plays white by default
    int _aiDepth = 3;  // Search depth
	
public:
    void enableAI(bool enable, int aiColor = 2, int depth = 3);
    bool isAIEnabled() const { return _isAIEnabled; }
    bool isAITurn() const { return _isAIEnabled && _currentPlayer == _aiColor; }
    bool makeAIMove();

public:
	Game(int rows, int cols, Board *board);
	~Game();
	void render();
	bool handleInput();

	void resetGame();
	void passTurn();
	void DetermineTerritories();

	void setCurrentPlayer(const int &currentPlayer) { _currentPlayer = currentPlayer; }
	void setScorePlayer1(const int &scorePlayer1) { _scorePlayer1 = scorePlayer1; }
	void setScorePlayer2(const int &scorePlayer2) { _scorePlayer2 = scorePlayer2; }

	int getCurrentPlayer() const { return _currentPlayer; }
	int getScorePlayer1() const { return _scorePlayer1; }
	int getScorePlayer2() const { return _scorePlayer2; }
	int getFinalScorePlayer1() const { return _FinalScorePlayer1; }
	int getFinalScorePlayer2() const { return _FinalScorePlayer2; }
	bool isGameOver() const { return _isGameOver; }
	std::string getGameMode() const { return _gameMode; }

	std::vector<std::vector<int>> getGrid();
	
    // Async AI support
    std::pair<int, int> calculateAIMove();  // Runs on background thread
    bool applyAIMove(int row, int col);     // Runs on main thread

	// Access to GroupManager for AI
    GroupManager& getGroupManager() { return _groupManager; }

    // For AI: make/undo moves efficiently
    bool tryMove(int row, int col, int color, std::vector<std::pair<int,int>>& captured);
    void undoMoveAI(int row, int col, int color, const std::vector<std::pair<int,int>>& captured);

private:
	Board *_board = nullptr;
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

    // Optimized group management
    GroupManager _groupManager;

    // Ko tracking (simple: last captured single stone position)
    bool _hasKo = false;
    int _koRow = -1, _koCol = -1;

    // Sync GroupManager state with Board (for rendering)
    void syncBoardFromGroupManager();
};
