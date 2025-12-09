#pragma once

#include <utility>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <future>

class GroupManager;
class Game;

class GoAI {
public:
	GoAI(Game *game, GroupManager *groupManager, bool isAB);

	int evaluate();
	bool isInvalidMove(int row, int col, int color);
	std::pair<int, int> findBestMove(int color, int dep);
	int minimax(int color, int dep, int alpha, int beta, bool isMax);
    int moveHeuristic(int row, int col, int color);
	void sync();
	std::vector<std::pair<int, int>> getOpeningMoves();
	bool isCornerTaken(int cornerRow, int cornerCol, int radius = 3);
	int countStones();
	std::pair<int, int> getBestOpeningMove(int color);
	bool isMyEye(int row, int col, int color);

private:
	GroupManager *_groupManager;
	Game *_game;

	int _aiColor;
	bool _isAB;
	int _koRow;
	int _koCol;
	bool _hasKo;
	int _rows;
	int _cols;
    
    std::unordered_map<uint64_t, int> _transpositionTable;

	static constexpr int dx[4] = {1, 0, -1, 0};
	static constexpr int dy[4] = {0, 1, 0, -1};

private:
	static constexpr int MAX_TIME_MS = 500;
	std::chrono::high_resolution_clock::time_point _timeLimit;
	bool _timeOut = false;
	void startTimer();
	inline bool checkTimeout() { return std::chrono::high_resolution_clock::now() >= _timeLimit; }
};
