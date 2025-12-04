#include "GoAI.h"
#include "Game.h"
#include "GroupManager.h"

#include <iostream>
#include <algorithm>

const int INF = 1e9;

GoAI::GoAI(Game *game, GroupManager *groupManager, bool isAB) {
	_game = game;
	_rows = game->getRow();
	_cols = game->getCol();
	_groupManager = groupManager;
	_isAB = isAB;
}

void GoAI::sync() {
	_koRow = _game->getKoRow();
	_koCol = _game->getKoCol();
	_hasKo = _game->getHasKo();
}

bool GoAI::isInvalidMove(int row, int col, int color) {
	return (_hasKo && _koRow == row && _koCol == col) || _groupManager->isSelfCaptured(row, col, color);
}

std::pair<int, int> GoAI::findBestMove(int color, int dep) {
	auto moves = _groupManager->getValidMoves(2, color);
	if (moves.empty()) return {-1, -1};

	sync();
	std::pair<int, int> bestMove = {-1, -1};
	int bestValue = -INF;

	for (auto [row, col] : moves) {
		if (isInvalidMove(row, col, color)) continue;
		if (!_groupManager->applyMove(row, col, color)) {
			_groupManager->undo();
			continue;
		}

		int curValue = minimax(3 - color, dep - 1, -INF, INF, false);
		if (curValue > bestValue) {
			bestMove = {row, col};
			bestValue = curValue;
		}

		_groupManager->undo();
	}
	return bestMove;
}

int GoAI::minimax(int color, int dep, int alpha, int beta, bool isMax) {
	if (!dep) return evaluate(color);

	auto moves = _groupManager->getValidMoves(2, color);
	if (moves.empty()) return evaluate(color);
	int bestValue = isMax ? -INF : INF;

	for (auto [row, col] : moves) {
		if (isInvalidMove(row, col, color)) continue;
		if (!_groupManager->applyMove(row, col, color)) {
			_groupManager->undo();
			continue;
		}

		int curValue = minimax(3 - color, dep - 1, alpha, beta, !isMax);
		_groupManager->undo();

		if (isMax) {
			bestValue = std::max(bestValue, curValue);
			alpha = std::max(alpha, curValue);
		} else {
			bestValue = std::min(bestValue, curValue);
			beta = std::min(beta, curValue);
		}

		if (_isAB && beta <= alpha) {
			break;
		}
	}

	if (bestValue == INF || bestValue == -INF) return evaluate(color);

	return bestValue;
}

int GoAI::evaluate(int color) {
	int oppColor = 3 - color;
	int aiScore = 0;
	int oppScore = 0;
	int score = 0;

	for (int row = 0; row < _rows; row++) {
		for (int col = 0; col < _cols; col++) {
			int cur = _groupManager->getValue(row, col);
			if (cur == color)
				aiScore++;
			else if (cur == oppColor)
				oppScore++;
		}
	}
	score += (aiScore - oppScore) * 10;

	aiScore = _groupManager->getTerritory(color);
	oppScore = _groupManager->getTerritory(oppColor);
	score += (aiScore - oppScore) * 5;

	aiScore = oppScore = 0;
	for (int row = 0; row < _rows; row++) {
		for (int col = 0; col < _cols; col++) {
			int cur = _groupManager->getValue(row, col);
			if (!cur || !_groupManager->isRoot(row, col)) continue;
			if (cur == color)
				aiScore += _groupManager->getGroupLiberties(row, col);
			else if (cur == oppColor)
				oppScore += _groupManager->getGroupLiberties(row, col);
		}
	}
	score += (aiScore - oppScore) * 2;

	int centerR = _rows >> 1;
	int centerC = _cols >> 1;
	aiScore = oppScore = 0;
	for (int row = 0; row < _rows; row++) {
		for (int col = 0; col < _cols; col++) {
			int cur = _groupManager->getValue(row, col);
			int dis = std::abs(row - centerR) + std::abs(col - centerC);
			if (cur == color)
				aiScore += std::max(0, 10 - dis);
			else if (cur == oppColor)
				oppScore += std::max(0, 10 - dis);
		}
	}
	score += (aiScore - oppScore) * 1;

	return score;
}
