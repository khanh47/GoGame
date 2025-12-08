#include "GoAI.h"
#include "Game.h"
#include "GroupManager.h"

#include <iostream>
#include <algorithm>

const int INF = 1e9;
//const int LIM[3] {60, 45, 35}; // 9x9 - 13x13 - 19x19 - dep:6
const int LIM[3] {60, 50, 40}; // 9x9 - 13x13 - 19x19 - dep:5

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

bool GoAI::isMyEye(int row, int col, int color) {
	for (int i = 0; i < 4; i++) {
		int nr = row + dx[i];
		int nc = col + dy[i];
		if (_groupManager->isOutside(nr, nc)) {
			continue;
		}
		if (_groupManager->getValue(nr, nc) != color) return false;
	}
	int opponent = 3 - color;
	int enemyDiagonals = 0;
	const int diagX[] = {-1, -1, 1, 1};
	const int diagY[] = {-1, 1, -1, 1};

	for (int i = 0; i < 4; i++) {
		int nr = row + diagX[i];
		int nc = col + diagY[i];
		if (_groupManager->isOutside(nr, nc)) continue;
		if (_groupManager->getValue(nr, nc) == opponent) enemyDiagonals++;
	}

	if (row == 0 || row == _rows - 1 || col == 0 || col == _cols - 1) {
		if (enemyDiagonals >= 1) return false;
	} else {
		if (enemyDiagonals >= 2) return false;
	}
	return true;
}

bool GoAI::isInvalidMove(int row, int col, int color) {
    return (_hasKo && _koRow == row && _koCol == col);
}

int GoAI::moveHeuristic(int row, int col, int color) {
	if (isMyEye(row, col, color)) return -1e6;
	int score = 0;
	int opp = 3 - color;

	for (int i = 0; i < 4; i++) {
		int nr = row + dx[i];
		int nc = col + dy[i];
		if (_groupManager->isOutside(nr, nc)) continue;

		int val = _groupManager->getValue(nr, nc);
		if (val == opp) {
			int libs = _groupManager->getGroupLiberties(nr, nc);
			if (libs == 1) score += 1000;
			else if (libs == 2) score += 100;
		} else if (val == color) {
			int libs = _groupManager->getGroupLiberties(nr, nc);
			if (libs == 1) score += 800;
			else if (libs == 2) score += 50;
		}
	}

	int centerR = _rows >> 1;
	int centerC = _cols >> 1;
	int dist = std::abs(row - centerR) + std::abs(col - centerC);
	score += std::max(0, 10 - dist);

//	int lastR = _game->getLastRow();
//	int lastC = _game->getLastCol();
//	if (lastR != -1 && lastC != -1) {
//		int dist = std::abs(row - lastR) + std::abs(col - lastC);
//		if (dist <= 2) score += 1000;
//		else if (dist <= 4) score += 500;
//	}
	return score;
}

std::vector<std::pair<int, int>> GoAI::getOpeningMoves() {
	if (_rows == 9) {
		return {{2, 2}, {2, 6}, {6, 2}, {6, 6}, {4, 4}};
	}
	if (_rows == 13) {
		return {{3, 3}, {3, 9}, {9, 3}, {9, 9}, {6, 6},
			{3, 6}, {9, 6}, {6, 3}, {6, 9}};
	}
	if (_rows == 19) {
		return {{3, 3}, {3, 15}, {15, 3}, {15, 15}, {9, 9},
			{3, 9}, {15, 9}, {9, 3}, {9, 15}};
	}
	return {};
}

bool GoAI::isCornerTaken(int cornerRow, int cornerCol, int radius) {
	for (int dr = -radius; dr <= radius; dr++) {
		for (int dc = -radius; dc <= radius; dc++) {
			int r = cornerRow + dr;
			int c = cornerCol + dc;
			if (r < 0 || r >= _rows || c < 0 || c >= _cols) continue;
			if (_groupManager->getValue(r, c) != 0) {
				return true;
			}
		}
	}
	return false;
}

int GoAI::countStones() {
	int count = 0;
	for (int r = 0; r < _rows; r++) {
		for (int c = 0; c < _cols; c++) {
			if (_groupManager->getValue(r, c) != 0) {
				count++;
			}
		}
	}
	return count;
}

std::pair<int, int> GoAI::getBestOpeningMove(int color) {
	int opp = 3 - color;
	auto starPoints = getOpeningMoves();
	const int radius = 3;
	for (int r = 0; r < _rows; r++) {
		for (int c = 0; c < _cols; c++) {
			if (_groupManager->getValue(r, c) == color) {
				for (int dr = -radius; dr <= radius; dr++) {
					for (int dc = -radius; dc <= radius; dc++) {
						int nr = r + dr, nc = c + dc;
						if (nr < 0 || nr >= _rows || nc < 0 || nc >= _cols) continue;
						if (_groupManager->getValue(nr, nc) == opp) {
							return {-1, -1};
						}
					}
				}
			}
		}
	}
	for (auto [r, c] : starPoints) {
		if (!isCornerTaken(r, c, radius)) {
			return {r, c};
		}
	}
	return {-1, -1};
}

std::pair<int, int> GoAI::findBestMove(int color, int dep) {
	int stoneCount = countStones();

	int openingThreshold = (_rows == 19) ? 20 :
		(_rows == 13) ? 12 : 8;

	if (stoneCount < openingThreshold) {
		auto move = getBestOpeningMove(color);
		if (move.first != -1) {
			return move;
		}
	}

	auto moves = _groupManager->getValidMovesAtRoot(2, color);
	if (moves.empty()) return {-1, -1};
	_aiColor = color;
	_dep = dep;
	_transpositionTable.clear();
	count = 0;

	std::sort(moves.begin(), moves.end(), [&](auto& a, auto& b) {
		return moveHeuristic(a.first, a.second, color) >
		moveHeuristic(b.first, b.second, color);
	});

	sync();
	std::pair<int, int> bestMove = {-1, -1};
	int bestValue = -INF;
	int cnt = 0;
	if (_dep > 4) {
		if (_rows > 9) cnt++;
		if (_rows > 13) cnt++;
	}
	for (int r = 0; r < _rows; r++) {
		for (int c = 0; c < _cols; c++) {
			if (_groupManager->getValue(r, c) && _groupManager->isRoot(r, c)) {
				std::cout << _groupManager->getGroupSize(r, c) << ' ' << r << ' ' << c << std::endl;
			}
		}
	}

	for (int i = 0; i < std::min(LIM[cnt], (int) moves.size()); i++) {
		auto [row, col] = moves[i];
		if (isInvalidMove(row, col, color)) continue;
		if (!_groupManager->applyMove(row, col, color)) {
			_groupManager->undo();
			continue;
		}
		std::cout << row << ' ' << col << std::endl;

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
	if (!dep) return evaluate();
	//if (!dep) return quiescenceSearch(color, alpha, beta, isMax, 4, -1, -1);

	uint64_t hash = _groupManager->getHash();

	auto it = _transpositionTable.find(hash);
	if (it != _transpositionTable.end()) {
		return it->second;
	}

	auto moves = _groupManager->getValidMoves(2, color);
	if (moves.empty()) return evaluate();
	int bestValue = isMax ? -INF : INF;

	std::sort(moves.begin(), moves.end(), [&](auto& a, auto& b) {
		return moveHeuristic(a.first, a.second, color) >
		moveHeuristic(b.first, b.second, color);
	});
	int cnt = 0;
	if (_dep > 4) {
		if (_rows > 9) cnt++;
		if (_rows > 13) cnt++;
	}

	for (int i = 0; i < std::min(LIM[cnt], (int) moves.size()); i++) {
		auto [row, col] = moves[i];
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

	if (bestValue == INF || bestValue == -INF) return evaluate();

	_transpositionTable[hash] = bestValue;
	return bestValue;
}

int GoAI::quiescenceSearch(int color, int alpha, int beta, bool isMax, int qs_depth, int lastR, int lastC) {
	if (qs_depth <= 0) return evaluate();

	int stand_pat = evaluate();

	if (isMax) {
		if (stand_pat >= beta) return beta;
		if (stand_pat > alpha) alpha = stand_pat;
	} else {
		if (stand_pat <= alpha) return alpha;
		if (stand_pat < beta) beta = stand_pat;
	}

	std::vector<std::pair<int, int>> moves;

	if (lastR != -1) {
		moves = _groupManager->getLocalQSMoves(lastR, lastC, color);
	} else {
		moves = _groupManager->getQSMoves(color);
	}
	std::sort(moves.begin(), moves.end(), [&](auto& a, auto& b) {
		return moveHeuristic(a.first, a.second, color) >
		moveHeuristic(b.first, b.second, color);
	});

	int cnt = 0;
	if (_dep > 4) {
		if (_rows > 9) cnt++;
		if (_rows > 13) cnt++;
	}

	for (int i = 0; i < std::min(LIM[cnt], (int) moves.size()); i++) {
		auto [row, col] = moves[i];
		if (moveHeuristic(row, col, color) < 300) break;
		if (isInvalidMove(row, col, color)) continue;

		if (!_groupManager->applyMove(row, col, color)) {
			_groupManager->undo();
			continue;
		}

		int score = quiescenceSearch(3 - color, alpha, beta, !isMax, qs_depth - 1, row, col);
		_groupManager->undo();

		if (isMax) {
			if (score >= beta) return beta;
			if (score > alpha) alpha = score;
		} else {
			if (score <= alpha) return alpha;
			if (score < beta) beta = score;
		}
	}

	return isMax ? alpha : beta;
}

int GoAI::evaluate() {
	int myColor = _aiColor;
	int oppColor = 3 - _aiColor;
	int score = 0;

	for (int row = 0; row < _rows; row++) {
		for (int col = 0; col < _cols; col++) {
			int cur = _groupManager->getValue(row, col);
			if (!cur) continue;
			if (!_groupManager->isRoot(row, col)) continue;
			int val = 0;
			int size = _groupManager->getGroupSize(row, col);
			int libs = _groupManager->getGroupLiberties(row, col);

			val += size * 10;
			val += libs * 2;
			val -= 200 * (libs == 1);

			if (cur == myColor) {
				score += val;
			} else {
				score -= val;
			}
		}
	}

	auto [myTerritory, oppTerritory] = _groupManager->getTerritory(myColor);
	//auto [myTerritory, oppTerritory, myAliveGroup, oppAliveGroup] = _groupManager->getAliveGroupAndTerritory(myColor);
	score += (myTerritory - oppTerritory) * 15;
	//score += (myAliveGroup - oppAliveGroup) * 90;

return score;
}
