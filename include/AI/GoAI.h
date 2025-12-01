#pragma once
#include "GroupManager.h"
#include <vector>
#include <utility>
#include <limits>
#include <memory>

class Board;
class Game;

class GoAI {
public:
    GoAI(int rows, int cols);
    ~GoAI() = default;

    // Find best move for the AI player
    // Returns {row, col} of best move, or {-1, -1} if should pass
    std::pair<int, int> findBestMove(Game* game, int aiColor, int depth);

private:
    int _rows, _cols;
    
    // Internal GroupManager for simulation (separate from game's)
    std::unique_ptr<GroupManager> _simManager;

    // Minimax search
    int minimax(int depth, int currentPlayer, int aiColor, 
                int alpha, int beta, bool useAlphaBeta = true);

    // Generate candidate moves (proximity-based)
    std::vector<std::pair<int, int>> generateCandidates(int radius = 2);

    // Evaluate board position (higher = better for aiColor)
    int evaluate(int aiColor);

    // Helper
    bool isInside(int r, int c) const { return r >= 0 && r < _rows && c >= 0 && c < _cols; }
};