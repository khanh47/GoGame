#include "Game.h"
#include "Board.h"
#include "colors.h"
#include "raylib.h"
#include <cctype>
#include <cmath>
#include <queue>
#include <vector>

using namespace std;

const int dx[8] = {0, 0, -1, 1, -1, 1, -1, 1};
const int dy[8] = {-1, 1, 0, 0, 1, -1, -1, 1};

Game::Game(int rows, int cols, Board *board) : _rows(rows), _cols(cols), _board(board) {}

Game::~Game() {}

void Game::passTurn() {
	_currentPlayer = _currentPlayer == 1 ? 2 : 1;
	if (_isLastTurnPass)
		_isGameOver = true;
	_isLastTurnPass = true;
}

void Game::render() {
	_board->render();
	Vector2 mousePos = GetMousePosition();
	int row = (int)round((mousePos.x - CELL_SIZE - PADDING) / CELL_SIZE);
	int col = (int)round((mousePos.y - CELL_SIZE - PADDING) / CELL_SIZE);
	if (_board->isCellValid(row, col, _currentPlayer)) {
		_board->renderGhostStones(row, col, _currentPlayer);
	}
}

void Game::resetGame() {
	_board->init();
	_isLastTurnPass = false;
	_isGameOver = false;
	_currentPlayer = 1;
	_scorePlayer1 = 0;
	_scorePlayer2 = 0;
	_FinalScorePlayer1 = 0;
	_FinalScorePlayer2 = 0;
}

std::vector<std::vector<int>> Game::getGrid() { return _board->getGrid(); }

std::vector<std::vector<int>> Game::getValidPlayer1Map() { return _board->getValidPlayer1Map(); }

std::vector<std::vector<int>> Game::getValidPlayer2Map() { return _board->getValidPlayer2Map(); }

void Game::DetermineTerritories() {
	vector<vector<bool>> visited(_board->GetNumRows(), vector<bool>(_board->GetNumCols(), false));
	vector<pair<int, int>> Stones;
	queue<pair<int, int>> q;

	for (int i = 0; i < _board->GetNumRows(); i++) {
		for (int j = 0; j < _board->GetNumCols(); j++) {
			if (visited[i][j] || _board->getValue(i, j) != 0)
				continue;

			bool CurrentPlayer = false;
			bool OpponentPlayer = false;
			Stones.clear();

			q.push({i, j});
			visited[i][j] = true;

			while (!q.empty()) {
				auto [x, y] = q.front();
				Stones.push_back({x, y});
				q.pop();

				for (int k = 0; k < 4; k++) {
					int newX = x + dx[k];
					int newY = y + dy[k];

					if (_board->isCellInside(newX, newY) && !visited[newX][newY]) {
						if (_board->getValue(newX, newY) == 0) {
							visited[newX][newY] = true;
							q.push({newX, newY});
						} else if (_board->getValue(newX, newY) == _currentPlayer) {
							CurrentPlayer = true;
						} else
							OpponentPlayer = true;
					}
				}
			}

			if (OpponentPlayer && CurrentPlayer)
				continue;
			else if (_currentPlayer == 1) {
				if (CurrentPlayer)
					_FinalScorePlayer1 += Stones.size();
				else if (OpponentPlayer)
					_FinalScorePlayer2 += Stones.size();
			} else if (_currentPlayer == 2) {
				if (CurrentPlayer)
					_FinalScorePlayer2 += Stones.size();
				else if (OpponentPlayer)
					_FinalScorePlayer1 += Stones.size();
			}
		}
	}
}

void Game::SelfCapture(int CurrentPlayer, vector<pair<int, int>> Stones, vector<pair<int, int>> EmptySpots) {
	vector<vector<bool>> visited(_board->GetNumRows(), vector<bool>(_board->GetNumCols(), false));
	queue<pair<int, int>> q;
	bool isCaptured = true;
	bool OpponentCaptured = false;
	int OpponentPlayer;

	if (CurrentPlayer == 1)
		OpponentPlayer = 2;
	else
		OpponentPlayer = 1;

	for (auto [x, y] : Stones) {
		q.push({x, y});
		visited[x][y] = true;
	}

	q.push({EmptySpots[0].first, EmptySpots[0].second});
	visited[EmptySpots[0].first][EmptySpots[0].second] = true;

	vector<vector<bool>> vis(_board->GetNumRows(), vector<bool>(_board->GetNumCols(), false));
	vis[EmptySpots[0].first][EmptySpots[0].second] = true;

	for (int i = 0; i < 4; i++) {
		int newX = EmptySpots[0].first + dx[i];
		int newY = EmptySpots[0].second + dy[i];
		bool TempOpponentCaptured = true;
		bool ExistsOpponentStone = false;

		if (_board->isCellInside(newX, newY) && _board->getValue(newX, newY) == OpponentPlayer && !vis[newX][newY]) {
			queue<pair<int, int>> Current;
			ExistsOpponentStone = true;

			Current.push({newX, newY});
			vis[newX][newY] = true;

			while (!Current.empty()) {
				auto [x, y] = Current.front();
				Current.pop();

				for (int j = 0; j < 4; j++) {
					int u = x + dx[j];
					int v = y + dy[j];

					if (_board->isCellInside(u, v)) {
						if (_board->getValue(u, v) == OpponentPlayer && !vis[u][v]) {
							vis[u][v] = true;
							Current.push({u, v});
						} else if (_board->getValue(u, v) == 0 && !vis[u][v]) {
							TempOpponentCaptured = false;
						}
					}
				}
			}
		}

		if (TempOpponentCaptured && ExistsOpponentStone) {
			OpponentCaptured = true;
			break;
		}
	}

	if (OpponentCaptured) {
		if (CurrentPlayer == 1)
			_board->setValidPlayer1(EmptySpots[0].first, EmptySpots[0].second, 0);
		else if (CurrentPlayer == 2)
			_board->setValidPlayer2(EmptySpots[0].first, EmptySpots[0].second, 0);
		return;
	}

	while (!q.empty()) {
		auto [x, y] = q.front();
		q.pop();

		for (int i = 0; i < 4; i++) {
			int newX = x + dx[i];
			int newY = y + dy[i];

			if (_board->isCellInside(newX, newY)) {
				if (_board->getValue(newX, newY) == CurrentPlayer && !visited[newX][newY]) {
					visited[newX][newY] = true;
					q.push({newX, newY});
				} else if (_board->getValue(newX, newY) == 0 && !visited[newX][newY]) {
					isCaptured = false;
				}
			}
		}
	}

	if (isCaptured) {
		if (CurrentPlayer == 1)
			_board->setValidPlayer1(EmptySpots[0].first, EmptySpots[0].second, 1);
		else if (CurrentPlayer == 2)
			_board->setValidPlayer2(EmptySpots[0].first, EmptySpots[0].second, 1);
	} else {
		if (CurrentPlayer == 1)
			_board->setValidPlayer1(EmptySpots[0].first, EmptySpots[0].second, 0);
		else if (CurrentPlayer == 2)
			_board->setValidPlayer2(EmptySpots[0].first, EmptySpots[0].second, 0);
	}
}

void Game::CheckValid(int CurrentPlayer, int OpponentPlayer) {
	vector<vector<bool>> visited(_board->GetNumRows(), vector<bool>(_board->GetNumCols(), false));
	vector<pair<int, int>> Stones;
	vector<pair<int, int>> EmptySpots;
	vector<pair<int, int>> OpponentStones;

	for (int i = 0; i < _board->GetNumRows(); i++) {
		for (int j = 0; j < _board->GetNumCols(); j++) {
			if (_board->getValue(i, j) != CurrentPlayer || visited[i][j])
				continue;

			queue<pair<int, int>> q;

			q.push({i, j});
			visited[i][j] = true;
			Stones.clear();
			EmptySpots.clear();
			OpponentStones.clear();

			while (!q.empty()) {
				auto [x, y] = q.front();
				Stones.push_back({x, y});
				q.pop();

				for (int k = 0; k < 4; k++) {
					int newX = x + dx[k];
					int newY = y + dy[k];

					if (_board->isCellInside(newX, newY)) {
						if (_board->getValue(newX, newY) == 0) {
							EmptySpots.push_back({newX, newY});
						} else if (_board->getValue(newX, newY) == CurrentPlayer && !visited[newX][newY]) {
							q.push({newX, newY});
							visited[newX][newY] = true;
						}
					}
				}
			}

			if (EmptySpots.size() == 1 && !_board->Alive(EmptySpots[0].first, EmptySpots[0].second)) {
				if ((CurrentPlayer == 1 && _board->getValidPlayer1(EmptySpots[0].first, EmptySpots[0].second) == 0) ||
						(CurrentPlayer == 2 && _board->getValidPlayer2(EmptySpots[0].first, EmptySpots[0].second) == 0))
					SelfCapture(CurrentPlayer, Stones, EmptySpots);
			}
		}
	}

	for (int i = 0; i < _board->GetNumRows(); i++) {
		for (int j = 0; j < _board->GetNumCols(); j++) {
			if (_board->getValue(i, j) == 0 && !_board->Alive(i, j)) {
				if (CurrentPlayer == 1 && _board->getValidPlayer1(i, j))
					continue;
				if (CurrentPlayer == 2 && _board->getValidPlayer2(i, j))
					continue;

				Stones.clear();
				EmptySpots.clear();
				EmptySpots.push_back({i, j});
				SelfCapture(CurrentPlayer, Stones, EmptySpots);
			}
		}
	}
}

void Game::BeingCaptured(int CurrentPlayer, int OpponentPlayer, pair<int, int> lastMove) {
	vector<vector<bool>> visited(_board->GetNumRows(), vector<bool>(_board->GetNumCols(), false));
	vector<pair<int, int>> Stones;
	vector<pair<int, int>> OpponentStones;

	for (int i = 0; i < _board->GetNumRows(); i++) {
		for (int j = 0; j < _board->GetNumCols(); j++) {
			if (_board->getValue(i, j) != CurrentPlayer || visited[i][j])
				continue;

			queue<pair<int, int>> q;
			bool isCaptured = true;

			q.push({i, j});
			visited[i][j] = true;
			Stones.clear();
			OpponentStones.clear();

			while (!q.empty()) {
				auto [x, y] = q.front();
				Stones.push_back({x, y});
				q.pop();

				if (x == lastMove.first && y == lastMove.second && _board->getValue(x, y) == CurrentPlayer)
					isCaptured = false;

				for (int k = 0; k < 4; k++) {
					int newX = x + dx[k];
					int newY = y + dy[k];

					if (_board->isCellInside(newX, newY)) {
						if (_board->getValue(newX, newY) == 0) {
							isCaptured = false;
						} else if (_board->getValue(newX, newY) == CurrentPlayer && !visited[newX][newY]) {
							visited[newX][newY] = true;
							q.push({newX, newY});
						} else if (_board->getValue(newX, newY) == OpponentPlayer) {
							OpponentStones.push_back({newX, newY});
						}
					}
				}
			}

			if (isCaptured) {
				for (auto [x, y] : Stones) {
					_board->setValue(x, y, 0);
					_board->setValidPlayer1(x, y, 0);
					_board->setValidPlayer2(x, y, 0);
				}

				if (CurrentPlayer == 1) {
					_scorePlayer2 += Stones.size();
				} else if (CurrentPlayer == 2) {
					_scorePlayer1 += Stones.size();
				}

				if (Stones.size() == 1) {
					int TotalCurrentPlayer = 0;
					int TotalInsideCells = 0;

					for (int i = 0; i < 4; i++) {
						int newX = lastMove.first + dx[i];
						int newY = lastMove.second + dy[i];

						if (_board->isCellInside(newX, newY)) {
							if (newX != Stones[0].first && newY != Stones[0].second) {
								TotalInsideCells++;

								if (_board->getValue(newX, newY) == CurrentPlayer)
									TotalCurrentPlayer++;
							}
						}
					}

					if (TotalCurrentPlayer == TotalInsideCells) {
						if (CurrentPlayer == 1) {
							_board->setValidPlayer1(Stones[0].first, Stones[0].second, 2);
						} else if (CurrentPlayer == 2) {
							_board->setValidPlayer2(Stones[0].first, Stones[0].second, 2);
						}
					}
				}
			} else {
				for (auto [x, y] : Stones) {
					_board->setValidPlayer1(x, y, 1);
					_board->setValidPlayer2(x, y, 1);
				}
			}
		}
	}
}

bool Game::handleInput() {
	if (_isGameOver)
		return false;
	Vector2 mousePos = GetMousePosition();
	bool isMouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

	int row = (int)round((mousePos.x - CELL_SIZE - PADDING) / CELL_SIZE);
	int col = (int)round((mousePos.y - CELL_SIZE - PADDING) / CELL_SIZE);
	bool isValidMove = 0;

	if (isMouseClicked && _board->isCellValid(row, col, _currentPlayer)) {
		_board->setValue(row, col, _currentPlayer);
		_board->setValidPlayer1(row, col, 1);
		_board->setValidPlayer2(row, col, 1);

		for (int i = 0; i < _board->GetNumRows(); i++) {
			for (int j = 0; j < _board->GetNumCols(); j++) {
				if (_currentPlayer == 1 && _board->getValidPlayer2(i, j) == 2) {
					if (_board->getValue(i, j) == 0)
						_board->setValidPlayer2(i, j, 0);
					else
						_board->setValidPlayer2(i, j, 1);
				} else if (_currentPlayer == 2 && _board->getValidPlayer1(i, j) == 2) {
					if (_board->getValue(i, j) == 0)
						_board->setValidPlayer1(i, j, 0);
					else
						_board->setValidPlayer1(i, j, 1);
				}
			}
		}

		_currentPlayer = ((_currentPlayer == 1) ? 2 : 1);
		isValidMove = true;
	}

	// Checking for captures

	BeingCaptured(1, 2, {row, col}); // Check for Black Stones being captured by White
	BeingCaptured(2, 1, {row, col}); // Check for White Stones being captured by Black
	CheckValid(1, 2);								 // Check for valid moves for Black
	CheckValid(2, 1);								 // Check for valid moves for White

	// When the Game is Over

	if (_isGameOver) {
		DetermineTerritories();
		_scorePlayer1 += _FinalScorePlayer1;
		_scorePlayer2 += _FinalScorePlayer2;
	}
	if (isValidMove)
		_isLastTurnPass = false;
	return isValidMove;
}
