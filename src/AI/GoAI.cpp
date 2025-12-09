#include "GoAI.h"
#include "Game.h"
#include "GroupManager.h"

#include <random>
#include <iostream>
#include <algorithm>

const int INF = 1e9;
const int LIM = 40;

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

	return enemyDiagonals < 2;
}

bool GoAI::isInvalidMove(int row, int col, int color) {
	return (_hasKo && _koRow == row && _koCol == col);
}

int GoAI::moveHeuristic(int row, int col, int color) {
    int score = 0;
    int opp = 3 - color;
	int ourLibs = 0;
	bool capture = false;
	bool isTemSafe = true;
	int lastGroup = -1;
    
    for (int i = 0; i < 4; i++) {
        int nr = row + dx[i];
        int nc = col + dy[i];
        if (_groupManager->isOutside(nr, nc)) continue;
        
        int val = _groupManager->getValue(nr, nc);
		if (!val) {
			isTemSafe = false;
			ourLibs++;
		} else if (val == opp) {
			isTemSafe = false;
            int libs = _groupManager->getGroupLiberties(nr, nc);
            if (libs == 1) {
				capture = true;
				score += 1000;
			} else if (libs == 2) score += 100;
        } else if (val == color) {
            int libs = _groupManager->getGroupLiberties(nr, nc);
			int id = _groupManager->getGroupID(nr, nc);
			if (id != lastGroup) {
				ourLibs += libs - 1;
				lastGroup = id;
			}
            if (libs == 1) {
				score += 800;
			} else if (libs == 2) score += 50;
        }
    }
	if (ourLibs <= 1 && !capture) score -= 1e4;
	if (ourLibs <= 2) score -= 50;
	if (isTemSafe) score -= 1000;
    
    int centerR = _rows >> 1;
    int centerC = _cols >> 1;
    int dist = std::abs(row - centerR) + std::abs(col - centerC);
    score += std::max(0, 10 - dist);
    
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
	const int radius = _rows == 9 ? 1 : 3;
	for (auto [r, c] : starPoints) {
		if (_groupManager->getValue(r, c) == color) {
			for (int dr = -radius; dr <= radius; dr++) {
				for (int dc = -radius; dc <= radius; dc++) {
					int nr = r + dr, nc = c + dc;
					if (nr < 0 || nr >= _rows || nc < 0 || nc >= _cols)
						continue;
					if (_groupManager->getValue(nr, nc) == opp) {
						return {-1, -1};
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

	int openingThreshold = (_rows == 19) ? 10 :
		(_rows == 13) ? 5 : 2;

	if (stoneCount < openingThreshold) {
		auto move = getBestOpeningMove(color);
		if (move.first != -1) {
			return move;
		}
	}
	startTimer();
	int stones = stoneCount;
	bool early = false;
	bool mid = false;
	bool late = false;

	float ratio = float(stones) / float(_rows * _cols);

	if (ratio < 0.40f) early = true;
	else if (ratio < 0.60f) mid = true;
	else late = true;

	std::cout << "In " << (early ? "early" : (mid ? "mid" : "end")) << " game currently\n";
	
	auto moves = _groupManager->getValidMovesAtRoot(2, color);
	if (moves.empty()) return {-1, -1};
	_aiColor = color;
	_transpositionTable.clear();
	count = 0;

	std::sort(moves.begin(), moves.end(), [&](auto& a, auto& b) {
		return moveHeuristic(a.first, a.second, color) >
		moveHeuristic(b.first, b.second, color);
	});

	sync();
	std::pair<int, int> bestMove = {-1, -1};
	int bestValue = -INF;
	int lim = dep == 4 ? moves.size() : LIM;

	for (auto [row, col] : moves) {
		if (_timeOut || checkTimeout())
            return bestMove;
		if (isInvalidMove(row, col, color) || isMyEye(row, col, color)) continue;
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
    // auto [myTerritory, oppTerritory, myAliveGroup, oppAliveGroup] = _groupManager->getAliveGroupAndTerritory(color);
    // myAliveGroup = _groupManager->getTerritory(color);
    // oppAliveGroup = _groupManager->getTerritory(3 - color);
    // std::cout << myTerritory << ' ' << oppTerritory << ' ' << myAliveGroup << ' ' << oppAliveGroup << std::endl;
	return bestMove;
}

int GoAI::minimax(int color, int dep, int alpha, int beta, bool isMax) {
    if (checkTimeout()) {
        _timeOut = true;
        return evaluate();
    }
	if (!dep) return evaluate();

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

	int lim = dep == 4 ? moves.size() : LIM;

	for (int i = 0; i < std::min(lim, (int) moves.size()); i++) {
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

int GoAI::evaluate() {
    int myColor = _aiColor;
    int oppColor = 3 - _aiColor;
    int score = 0;
    
    int myStones = 0, oppStones = 0;
    int myLiberties = 0, oppLiberties = 0;
    int myAtari = 0, oppAtari = 0;
    int myGroupCount = 0, oppGroupCount = 0;
    
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _cols; col++) {
            int cur = _groupManager->getValue(row, col);
            if (!cur) {
                continue;
            }
            
            if (cur == myColor) {
                myStones++;
                if (_groupManager->isRoot(row, col)) {
                    myGroupCount++;
                    int libs = _groupManager->getGroupLiberties(row, col);
                    myLiberties += libs;
                    if (libs == 1) myAtari++;
                }
            } else {
                oppStones++;
                if (_groupManager->isRoot(row, col)) {
                    oppGroupCount++;
                    int libs = _groupManager->getGroupLiberties(row, col);
                    oppLiberties += libs;
                    if (libs == 1) oppAtari++;
				}
            }
        }
    }
    
    auto [myTerritory, oppTerritory, myAliveGroup, oppAliveGroup] = _groupManager->getAliveGroupAndTerritory(myColor);
    
	score += (myLiberties - oppLiberties) * 2;
	score += (oppGroupCount - myGroupCount) * 5;
	score += (myStones - oppStones) * 10;
	// This for faster but dumper bot
	// score += (myTerritory - oppTerritory) * 15;
	// score += (oppAtari - myAtari) * 100;

	// This for slower but logical bot
	int stones = myStones + oppStones;
	bool early = false;
	bool mid = false;
	bool late = false;

	float ratio = float(stones) / float(_rows * _cols);

	if (ratio < 0.40f) early = true;
	else if (ratio < 0.60f) mid = true;
	else late = true;

	if (early) {
		score += (oppAtari - myAtari) * 100;
		score += (myStones - oppStones) * 20;
		score += (myTerritory - oppTerritory) * 15;
	} else if (mid) {
		score += (oppAtari - myAtari) * 50;
		score += (myAliveGroup - oppAliveGroup) * 100;
		score += (myTerritory - oppTerritory) * 20;
	} else if (late) {
		score += (oppAtari - myAtari) * 20;
		score += (myAliveGroup - oppAliveGroup) * 100;
		score += (myTerritory - oppTerritory) * 50;
	}

	return score;
}

void GoAI::startTimer() {
    _timeLimit = std::chrono::high_resolution_clock::now() +
                 std::chrono::milliseconds(MAX_TIME_MS);
    _timeOut = false;
}
