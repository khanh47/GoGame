#pragma once

#include <vector>
#include <utility>
#include <cstdint>
#include <unordered_map>

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
	std::pair<int, int> findBestMove(int color);

private:
	GroupManager *_groupManager;
	Game *_game;

	int count = 0;
	int _aiColor;
	bool _isAB;
	int _koRow;
	int _koCol;
	bool _hasKo;
	int _rows;
	int _cols;

	uint64_t _zobristTable[19][19][3];  // [row][col][color]
	uint64_t _currentHash = 0;

	std::unordered_map<uint64_t, int> _transpositionTable;

	void initZobrist();
	uint64_t computeHash() const;
};
