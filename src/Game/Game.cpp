#include "Game.h"
#include "Board.h"
#include "GoAI.h"
#include "colors.h"
#include "raylib.h"

#include <iostream>
#include <cctype>
#include <cmath>
#include <queue>
#include <vector>

using namespace std;

const int dx[4] = {0, 0, -1, 1};
const int dy[4] = {-1, 1, 0, 0};

Game::Game(int rows, int cols, Board *board)
    : _rows(rows), _cols(cols), _board(board), _groupManager(rows, cols) {}

Game::~Game() = default;  // unique_ptr handles cleanup

void Game::enableAI(bool enable, int aiColor, int depth) {
    _isAIEnabled = enable;
    _aiColor = aiColor;
    _aiDepth = depth;
    
    if (enable && ! _ai) {
        _ai = std::make_unique<GoAI>(_rows, _cols);
    }
}

std::pair<int, int> Game::calculateAIMove() {
    // This runs on a BACKGROUND thread
    // Only reads game state, doesn't modify it
    
    if (! _isAIEnabled || !_ai || _isGameOver) {
        return {-1, -1};
    }
    
    if (_currentPlayer != _aiColor) {
        return {-1, -1};
    }

    // Find best move (this is the slow part)
    return _ai->findBestMove(this, _aiColor, _aiDepth);
}

bool Game::applyAIMove(int row, int col) {
    // This runs on the MAIN thread
    
    if (!_isAIEnabled || _isGameOver) {
        return false;
    }

    if (_currentPlayer != _aiColor) {
        return false;
    }

    // AI passes
    if (row == -1 && col == -1) {
        passTurn();
        return true;
    }

    // Safety check: shouldn't happen if AI is correct
    if (_hasKo && row == _koRow && col == _koCol) {
        std::cerr << "Warning: AI tried to play Ko move!" << std::endl;
        passTurn();
        return true;
    }

    // Make the move
    auto [legal, capturedStones] = _groupManager.makeMove(row, col, _currentPlayer);

    if (! legal) {
        std::cerr << "Warning: AI picked illegal move at (" << row << "," << col << ")" << std::endl;
        passTurn();
        return true;
    }

    // Update scores
    if (_currentPlayer == 1) {
        _scorePlayer1 += capturedStones. size();
    } else {
        _scorePlayer2 += capturedStones. size();
    }

    // Ko detection
    if (capturedStones. size() == 1) {
        int capR = capturedStones[0].first;
        int capC = capturedStones[0].second;
        int myGroupSize = _groupManager. getGroupSize(row, col);
        int myGroupLibs = _groupManager. getGroupLiberties(row, col);

        if (myGroupSize == 1 && myGroupLibs == 1) {
            _hasKo = true;
            _koRow = capR;
            _koCol = capC;
        } else {
            _hasKo = false;
        }
    } else {
        _hasKo = false;
    }

    syncBoardFromGroupManager();
    _currentPlayer = (_currentPlayer == 1) ? 2 : 1;
    _isLastTurnPass = false;

    return true;
}

bool Game::makeAIMove() {
    if (!_isAIEnabled || ! _ai || _isGameOver) {
        return false;
    }
    
    if (_currentPlayer != _aiColor) {
        return false;  // Not AI's turn
    }

    // Find best move
    auto [row, col] = _ai->findBestMove(this, _aiColor, _aiDepth);

    if (row == -1 && col == -1) {
        // AI decides to pass
        passTurn();
        return true;
    }

    // Check Ko
    if (_hasKo && row == _koRow && col == _koCol) {
        // AI should not pick Ko move, but if it does, pass
        passTurn();
        return true;
    }

    // Make the move
    auto [legal, capturedStones] = _groupManager.makeMove(row, col, _currentPlayer);

    if (! legal) {
        // Shouldn't happen if AI is working correctly
        passTurn();
        return true;
    }

    // Update scores based on captures
    if (_currentPlayer == 1) {
        _scorePlayer1 += capturedStones.size();
    } else {
        _scorePlayer2 += capturedStones. size();
    }

    // Ko detection
    if (capturedStones.size() == 1) {
        int capR = capturedStones[0]. first;
        int capC = capturedStones[0].second;
        int myGroupSize = _groupManager.getGroupSize(row, col);
        int myGroupLibs = _groupManager.getGroupLiberties(row, col);

        if (myGroupSize == 1 && myGroupLibs == 1) {
            _hasKo = true;
            _koRow = capR;
            _koCol = capC;
        } else {
            _hasKo = false;
        }
    } else {
        _hasKo = false;
    }

    // Sync Board for rendering
    syncBoardFromGroupManager();

    // Switch player
    _currentPlayer = (_currentPlayer == 1) ? 2 : 1;
    _isLastTurnPass = false;

    return true;
}

void Game::passTurn() {
    _currentPlayer = _currentPlayer == 1 ? 2 : 1;
    if (_isLastTurnPass)
        _isGameOver = true;
    _isLastTurnPass = true;
    _hasKo = false;
}

void Game::render() {
    _board->render();
    Vector2 mousePos = GetMousePosition();
    int row = (int)round((mousePos.x - CELL_SIZE - PADDING) / CELL_SIZE);
    int col = (int)round((mousePos.y - CELL_SIZE - PADDING) / CELL_SIZE);

    // Bounds check - FIXED! 
    if (row < 0 || row >= _rows || col < 0 || col >= _cols) {
        return;
    }

    // Don't show ghost stone if it's AI's turn
    if (isAITurn()) {
        return;
    }

    // Check validity using GroupManager
    bool isValid = _groupManager.getValue(row, col) == 0 &&
                   ! _groupManager.wouldBeSelfCapture(row, col, _currentPlayer);

    // Check Ko
    if (_hasKo && row == _koRow && col == _koCol) {
        isValid = false;
    }

    if (isValid) {
        _board->renderGhostStones(row, col, _currentPlayer);
    }
}

void Game::resetGame() {
    _board->init();
    _groupManager.reset();
    _isLastTurnPass = false;
    _isGameOver = false;
    _currentPlayer = 1;
    _scorePlayer1 = 0;
    _scorePlayer2 = 0;
    _FinalScorePlayer1 = 0;
    _FinalScorePlayer2 = 0;
    _hasKo = false;
    _koRow = _koCol = -1;
}

std::vector<std::vector<int>> Game::getGrid() {
    return _board->getGrid();
}

void Game::syncBoardFromGroupManager() {
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            _board->setValue(i, j, _groupManager.getValue(i, j));
        }
    }
}

void Game::DetermineTerritories() {
    _FinalScorePlayer1 = _groupManager.countTerritory(1);
    _FinalScorePlayer2 = _groupManager. countTerritory(2);
}

bool Game::tryMove(int row, int col, int color, vector<pair<int,int>>& captured) {
    auto [legal, caps] = _groupManager.makeMove(row, col, color);
    captured = std::move(caps);
    return legal;
}

void Game::undoMoveAI(int row, int col, int color, const vector<pair<int,int>>& captured) {
    int opp = (color == 1) ? 2 : 1;
    _groupManager.setValue(row, col, 0);
    for (auto [r, c] : captured) {
        _groupManager.setValue(r, c, opp);
    }
    _groupManager.reset();
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            int val = _board->getValue(i, j);
            if (val != 0) {
                _groupManager.makeMove(i, j, val);
            }
        }
    }
}

bool Game::handleInput() {
    if (_isGameOver)
        return false;

    // If AI's turn, don't accept player input
    if (isAITurn()) {
		makeAIMove();
        return false;
    }

    Vector2 mousePos = GetMousePosition();
    bool isMouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    int row = (int)round((mousePos.x - CELL_SIZE - PADDING) / CELL_SIZE);
    int col = (int)round((mousePos.y - CELL_SIZE - PADDING) / CELL_SIZE);
    bool isValidMove = false;

    // Bounds check
    if (row < 0 || row >= _rows || col < 0 || col >= _cols) {
        return false;
    }

    // Check Ko
    if (_hasKo && row == _koRow && col == _koCol) {
        return false;
    }

    if (isMouseClicked && _groupManager.getValue(row, col) == 0) {
        auto [legal, capturedStones] = _groupManager.makeMove(row, col, _currentPlayer);

        if (legal) {
            isValidMove = true;

            if (_currentPlayer == 1) {
                _scorePlayer1 += capturedStones.size();
            } else {
                _scorePlayer2 += capturedStones. size();
            }

            // Ko detection
            if (capturedStones.size() == 1) {
                int capR = capturedStones[0].first;
                int capC = capturedStones[0].second;
                int myGroupSize = _groupManager.getGroupSize(row, col);
                int myGroupLibs = _groupManager.getGroupLiberties(row, col);

                if (myGroupSize == 1 && myGroupLibs == 1) {
                    _hasKo = true;
                    _koRow = capR;
                    _koCol = capC;
                } else {
                    _hasKo = false;
                }
            } else {
                _hasKo = false;
            }

            syncBoardFromGroupManager();
            _currentPlayer = (_currentPlayer == 1) ? 2 : 1;
            _isLastTurnPass = false;
        }
    }

    if (_isGameOver) {
        DetermineTerritories();
        _scorePlayer1 += _FinalScorePlayer1;
        _scorePlayer2 += _FinalScorePlayer2;
    }

    return isValidMove;
}