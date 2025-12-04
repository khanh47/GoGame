#pragma once
#include "GroupManager.h"
#include "Board.h"
#include "GoAI.h"

#include <vector>
#include <memory>

struct MoveRecord {
	int row;
	int col;
	int color;  // 1 = black, 2 = white
};

class Game {
public:
	Game(int rows, int cols);
	~Game() = default;
	
	void init();
	void render();
	bool handleInput();

	bool applyMove(int row, int col, bool isReplay);
	void makeAIMove();
	bool applyAIMove(int row, int col);
	std::pair<int, int> calculateAIMove();

	void reset();
	void passTurn();
	bool undo();
	bool redo();
	void trim();
	void setCurrentPlayer(const int &currentPlayer) { _currentPlayer = currentPlayer; }
	void setScorePlayer1(const int &scorePlayer1) { _scorePlayer1 = scorePlayer1; }
	void setScorePlayer2(const int &scorePlayer2) { _scorePlayer2 = scorePlayer2; }
	void setMoveIndex(const int &moveIndex) { _moveIndex = moveIndex; }
	void setBoardSize(const int &boardSize) { _rows = _cols = boardSize; }
	void setDepth(const int &dep) { _dep = dep; }
	void setMoveHistory(const std::vector<MoveRecord> &moveHistory) { _moveHistory = moveHistory; }
	void enableAI(bool isEnabled, int dep, bool isAB);

	int getCurrentPlayer() const { return _currentPlayer; }
	int getScorePlayer1() const { return _scorePlayer1; }
	int getScorePlayer2() const { return _scorePlayer2; }
	int getKoRow() const { return _koRow; }
	int getKoCol() const { return _koCol; }
	int getHasKo() const { return _hasKo; }
	int getRow() const { return _rows; }
	int getCol() const { return _cols; }
	int getMoveIndex() const { return _moveIndex; }
	int getDepth() const { return _dep; }
	int getBoardSize() const { return _rows; }
	std::vector<MoveRecord> getHistoryMove() const { return _moveHistory; }
	bool isGameOver() const { return _isGameOver; }
	bool isAITurn() { return _isAIEnabled && _currentPlayer == _aiColor; }
	// Uncomment this for testing AI
	// bool isAITurn() { return _isAIEnabled && _currentPlayer; }
	bool isAIEnabled() { return _isAIEnabled; }

	bool isOutside(int row, int col) { return row < 0 || row >= _rows || col < 0 || col >= _cols; }
	void sync();
    void replayToIndex(int targetIndex);

private:
	std::unique_ptr<Board> _board;
	std::unique_ptr<GroupManager> _groupManager;
	std::unique_ptr<GoAI> _goAI;

	int _rows = 0;
	int _cols = 0;
	bool _isAB = false;
	bool _isAIEnabled = false;
	int _dep = 0;
	std::vector<MoveRecord> _moveHistory;
	int _moveIndex = -1;

	int _currentPlayer = 1; // 1 for black, 2 for white
	int _scorePlayer1 = 0;
	int _scorePlayer2 = 0;
	int _aiColor = 2;

	bool _isLastTurnPass = false;
	bool _isGameOver = false;
	bool _hasKo = false;
	int _koRow = -1;
	int _koCol = -1;

	int _lastRow = -1;
	int _lastCol = -1;
};
