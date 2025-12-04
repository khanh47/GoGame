#pragma once

#include <utility>

class GroupManager;
class Game;

class GoAI {
public:
	GoAI(Game *game, GroupManager *groupManager, bool isAB);

	int evaluate(int color);
	bool isInvalidMove(int row, int col, int color);
	std::pair<int, int> findBestMove(int color, int dep);
	int minimax(int color, int dep, int alpha, int beta, bool isMax);
	void sync();

private:
	GroupManager *_groupManager;
	Game *_game;

	bool _isAB;
	int _koRow;
	int _koCol;
	bool _hasKo;
	int _rows;
	int _cols;
};
