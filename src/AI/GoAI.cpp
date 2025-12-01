#include "GoAI.h"
#include "Game.h"
#include <algorithm>
#include <cmath>

GoAI::GoAI(int rows, int cols) 
    : _rows(rows), _cols(cols), 
      _simManager(std::make_unique<GroupManager>(rows, cols)) {}

std::pair<int, int> GoAI::findBestMove(Game* game, int aiColor, int depth) {
    if (! game) return {-1, -1};
    
    // Copy current board state to simulation manager
    GroupManager& gameGM = game->getGroupManager();
    _simManager->reset();
    
    // Copy board state
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            int val = gameGM.getValue(i, j);
            if (val != 0) {
                _simManager->makeMove(i, j, val);
            }
        }
    }

    // Generate candidate moves
    auto candidates = generateCandidates(2);

    if (candidates.empty()) {
        return {-1, -1};  // Pass
    }

    std::pair<int, int> bestMove = {-1, -1};
    int bestScore = std::numeric_limits<int>::min();
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    for (auto& [r, c] : candidates) {
        // Skip if would be self-capture
        if (_simManager->wouldBeSelfCapture(r, c, aiColor)) {
            continue;
        }

        // Prepare move record for undo
        auto record = _simManager->prepareMoveRecord(r, c, aiColor);
        
        // Try the move
        if (! _simManager->applyMove(record)) {
            continue;  // Illegal move
        }

        // Run minimax from opponent's perspective
        int opponentColor = (aiColor == 1) ? 2 : 1;
        int score = minimax(depth - 1, opponentColor, aiColor, alpha, beta);

        // Undo the move
        _simManager->undoMove(record);

        if (score > bestScore) {
            bestScore = score;
            bestMove = {r, c};
        }
        
        // Alpha-beta pruning
        alpha = std::max(alpha, score);
    }

    return bestMove;
}

int GoAI::minimax(int depth, int currentPlayer, int aiColor, int alpha, int beta, bool useAlphaBeta) {
    // Base case: evaluate position
    if (depth == 0) {
        return evaluate(aiColor);
    }

    // Generate candidates
    auto candidates = generateCandidates(2);

    if (candidates.empty()) {
        return evaluate(aiColor);
    }

    bool isMaximizing = (currentPlayer == aiColor);
    int bestScore = isMaximizing ? std::numeric_limits<int>::min()
                                 : std::numeric_limits<int>::max();

    int nextPlayer = (currentPlayer == 1) ? 2 : 1;

    for (auto& [r, c] : candidates) {
        // Skip if would be self-capture
        if (_simManager->wouldBeSelfCapture(r, c, currentPlayer)) {
            continue;
        }

        // Prepare and apply move
        auto record = _simManager->prepareMoveRecord(r, c, currentPlayer);
        
        if (!_simManager->applyMove(record)) {
            continue;
        }

        // Recurse
        int score = minimax(depth - 1, nextPlayer, aiColor, alpha, beta, useAlphaBeta);

        // Undo
        _simManager->undoMove(record);

        // Update best score
        if (isMaximizing) {
            bestScore = std::max(bestScore, score);
            if (useAlphaBeta) {
                alpha = std::max(alpha, score);
                if (beta <= alpha) break;  // Beta cutoff
            }
        } else {
            bestScore = std::min(bestScore, score);
            if (useAlphaBeta) {
                beta = std::min(beta, score);
                if (beta <= alpha) break;  // Alpha cutoff
            }
        }
    }

    // If no legal moves found, return evaluation
    if (bestScore == std::numeric_limits<int>::min() ||
        bestScore == std::numeric_limits<int>::max()) {
        return evaluate(aiColor);
    }

    return bestScore;
}

std::vector<std::pair<int, int>> GoAI::generateCandidates(int radius) {
    return _simManager->generateCandidates(radius);
}

int GoAI::evaluate(int aiColor) {
    int oppColor = (aiColor == 1) ? 2 : 1;
    int score = 0;

    // 1. Stone count difference (weight: 10)
    int aiStones = 0, oppStones = 0;
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            int val = _simManager->getValue(i, j);
            if (val == aiColor) aiStones++;
            else if (val == oppColor) oppStones++;
        }
    }
    score += (aiStones - oppStones) * 10;

    // 2. Territory estimate (weight: 5)
    int aiTerritory = _simManager->countTerritory(aiColor);
    int oppTerritory = _simManager->countTerritory(oppColor);
    score += (aiTerritory - oppTerritory) * 5;

    // 3.  Liberty count (weight: 2)
    int aiLiberties = 0, oppLiberties = 0;
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            int val = _simManager->getValue(i, j);
            if (val == aiColor) {
                aiLiberties += _simManager->getGroupLiberties(i, j);
            } else if (val == oppColor) {
                oppLiberties += _simManager->getGroupLiberties(i, j);
            }
        }
    }
    score += (aiLiberties - oppLiberties) * 2;

    // 4. Position bonus (center is slightly better)
    int centerR = _rows / 2, centerC = _cols / 2;
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            int val = _simManager->getValue(i, j);
            if (val == aiColor) {
                int dist = std::abs(i - centerR) + std::abs(j - centerC);
                score += std::max(0, 10 - dist);
            } else if (val == oppColor) {
                int dist = std::abs(i - centerR) + std::abs(j - centerC);
                score -= std::max(0, 10 - dist);
            }
        }
    }

    return score;
}