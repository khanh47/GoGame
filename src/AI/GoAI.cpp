#include "GoAI.h"
#include "Game.h"
#include "GroupManager.h"

#include <random>
#include <iostream>
#include <algorithm>

const int INF = 1e9;

GoAI::GoAI(Game *game, GroupManager *groupManager, bool isAB) {
	_game = game;
	_rows = game->getRow();
	_cols = game->getCol();
	_groupManager = groupManager;
	_isAB = isAB;
	initZobrist();
}

void GoAI::sync() {
	_koRow = _game->getKoRow();
	_koCol = _game->getKoCol();
	_hasKo = _game->getHasKo();
}

bool GoAI::isInvalidMove(int row, int col, int color) {
	return (_hasKo && _koRow == row && _koCol == col) || _groupManager->isSelfCaptured(row, col, color);
}

int GoAI::moveHeuristic(int row, int col, int color) {
    int score = 0;
    int opp = 3 - color;
    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};
    
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
    
    return score;
}

std::pair<int, int> GoAI::findBestMove(int color, int dep) {
	auto moves = _groupManager->getValidMoves(2, color);
	if (moves.empty()) return {-1, -1};
	_aiColor = color;
	_transpositionTable.clear();

    std::sort(moves.begin(), moves.end(), [&](auto& a, auto& b) {
        return moveHeuristic(a.first, a.second, color) > 
               moveHeuristic(b.first, b.second, color);
    });

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
	if (!dep) return evaluate();

    uint64_t hash = computeHash();
    
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
            if (!cur) continue;
            
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
    
    int myTerritory = _groupManager->getTerritory(myColor);
    int oppTerritory = _groupManager->getTerritory(oppColor);
    
    score += (myStones - oppStones) * 10;
    score += (myTerritory - oppTerritory) * 15;
    score += (myLiberties - oppLiberties) * 2;
    score += (oppAtari - myAtari) * 100;
    score += (oppGroupCount - myGroupCount) * 5;

	return score;
}

void GoAI::initZobrist() {
    std::mt19937_64 rng(12345);
    for (int r = 0; r < 19; r++) {
        for (int c = 0; c < 19; c++) {
            for (int color = 0; color < 3; color++) {
                _zobristTable[r][c][color] = rng();
            }
        }
    }
}

uint64_t GoAI::computeHash() const {
    uint64_t hash = 0;
    
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            int color = _groupManager->getValue(r, c);
            hash ^= _zobristTable[r][c][color];
        }
    }
    
    return hash;
}