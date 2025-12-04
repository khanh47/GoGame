#include "Game.h"
#include "raylib.h"

#include <cmath>
#include <iostream>
#include <memory>

const int dx[4] = {0, 0, -1, 1};
const int dy[4] = {-1, 1, 0, 0};

Game::Game(int rows, int cols)
    : _rows(rows), _cols(cols) {
	_moveHistory.push_back({-1, -1});
	_moveIndex++;
}

void Game::init() {
	_board = std::make_unique<Board>(_rows, _cols);
	_groupManager = std::make_unique<GroupManager>(_rows, _cols);
}

void Game::enableAI(bool isEnabled, int dep, bool isAB) {
	_isAIEnabled = isEnabled;
	_dep = dep;
	_isAB = isAB;
	_goAI = std::make_unique<GoAI>(this, _groupManager.get(), isAB);
}

void Game::render() {
    _board->render();
    if (_lastRow >= 0 && _lastCol >= 0) {
        float x = PADDING + (_lastRow + 1) * _board->getCellSize();
        float y = PADDING + (_lastCol + 1) * _board->getCellSize();
        
        DrawCircle(x, y, _board->getCellSize() * 0.1f, GRAY);
    }
    Vector2 mousePos = GetMousePosition();
    int row = (int)round((mousePos.x - _board->getCellSize() - PADDING) / _board->getCellSize());
    int col = (int)round((mousePos.y - _board->getCellSize() - PADDING) / _board->getCellSize());
	if (isAITurn())
		return;
	if (isOutside(row, col) || _board->getValue(row, col) || _groupManager->isSelfCaptured(row, col, _currentPlayer)) {
        return;
    }
	if (_hasKo && row == _koRow && col == _koCol) {
		return;
	}

	_board->renderGhostStones(row, col, _currentPlayer);
}

void Game::passTurn() {
    _currentPlayer = _currentPlayer == 1 ? 2 : 1;
    if (_isLastTurnPass) {
        _isGameOver = true;
		_scorePlayer1 += _groupManager->getTerritory(1);
		_scorePlayer2 += _groupManager->getTerritory(2);
	}
    _isLastTurnPass = true;
    _hasKo = false;
}

void Game::reset() {
    _board->reset();
	_groupManager->reset();
	_moveHistory.clear();

	_moveHistory.clear();
	_moveHistory.push_back({-1, -1});
	_moveIndex = 0;

	_hasKo = false;
	_koRow = _koCol = _lastRow = _lastCol -1;
    _isLastTurnPass = false;
    _isGameOver = false;
    _currentPlayer = 1;
    _scorePlayer1 = 0;
    _scorePlayer2 = 0;
}

void Game::replayToIndex(int targetIndex) {
    _groupManager->reset();
    _currentPlayer = 1;
    _scorePlayer1 = 0;
    _scorePlayer2 = 0;
    _hasKo = false;
	_koRow = _koCol = -1;
    _isLastTurnPass = false;
    _isGameOver = false;

    for (int i = 1; i <= targetIndex && i < _moveHistory.size(); i++) {
				auto [row, col, color] = _moveHistory[i];
				_currentPlayer = color;
				applyMove(row, col, true);
    }

    sync();
}

bool Game::undo() {
    if (!_moveIndex) return false;
	if (_isAIEnabled) {
		if (_moveIndex > 1) _moveIndex -= 2;
		else return false;
	} else {
		_moveIndex--;
	}
    
    replayToIndex(_moveIndex);
	return true;
}

bool Game::redo() {
    if (_moveIndex >= (int)_moveHistory.size() - 1) return false;
    _moveIndex++;
    replayToIndex(_moveIndex);
	return true;
}

void Game::sync() {
		for (int i = 0; i < _rows; i++) {
				for (int j = 0; j < _cols; j++) {
						_board->setValue(i, j, _groupManager->getValue(i, j));
				}
		}
}

std::pair<int, int> Game::calculateAIMove() {
	if (!isAITurn() || !_goAI || _isGameOver) return {-1, -1};
	return _goAI->findBestMove(_currentPlayer, _dep);
}


bool Game::applyAIMove(int row, int col) {
	if (!isAITurn() || !_goAI) return false;

	if (row == -1 && col == -1) {
		passTurn();
    return false;
	}

	if (_hasKo && row == _koRow && col == _koCol) {
		passTurn();
    return false;
	}

	return applyMove(row, col, false);
}

void Game::makeAIMove() {
	if (!isAITurn() || !_goAI) return;
	auto [row, col] = _goAI->findBestMove(_currentPlayer, _dep);

	if (row == -1 && col == -1) {
		passTurn();
    return;
	}

	if (_hasKo && row == _koRow && col == _koCol) {
		passTurn();
    return;
	}

	applyMove(row, col, false);
}

void Game::trim() {
	while (_moveHistory.size() - 1 > _moveIndex) _moveHistory.pop_back();
}

bool Game::applyMove(int row, int col, bool isReplay) {
	auto [valid, capturedStones] = _groupManager->makeMove(row, col, _currentPlayer);
	if (valid) {
		if (_currentPlayer == 1) {
			_scorePlayer1 += capturedStones.size();
		} else {
			_scorePlayer2 += capturedStones.size();
		}

		_hasKo = false;
		if (capturedStones.size() == 1) {
			if (_groupManager->getGroupSize(row, col) == 1 && _groupManager->getGroupLiberties(row, col) == 1) {
				_hasKo = true;
				_koRow = capturedStones[0].first;
				_koCol = capturedStones[0].second;
			}
		}
		sync();
		if (!isReplay) {
			trim();
			_moveHistory.push_back({row, col, _currentPlayer});
			_moveIndex++;
		}
		_currentPlayer = _currentPlayer == 1 ? 2 : 1;
		_isLastTurnPass = false;
		_lastRow = row;
		_lastCol = col;
	}
	return valid;
}

bool Game::handleInput() {
    if (_isGameOver || !_groupManager)
        return false;

    Vector2 mousePos = GetMousePosition();
    bool isMouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    int row = (int)round((mousePos.x - _board->getCellSize() - PADDING) / _board->getCellSize());
    int col = (int)round((mousePos.y - _board->getCellSize() - PADDING) / _board->getCellSize());

		if (isOutside(row, col)) {
        return false;
    }

		if (_hasKo && row == _koRow && col == _koCol) {
				return false;
		}

		if (isMouseClicked && !_groupManager->getValue(row, col)) {
				return applyMove(row, col, false);
		}
		return false;
}
