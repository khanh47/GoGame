#include "Game.h"
#include "raylib.h"
#include "Board.h"
#include "colors.h"
#include <cmath>
#include <iostream>
#include <cctype>
#include <vector>
#include <queue>

using namespace std;

const int dx[8] = {0, 0, -1, 1, -1, 1, -1, 1};
const int dy[8] = {-1, 1, 0, 0, 1, -1, -1, 1};

Game::Game(int rows, int cols) : _grid(std::make_unique<Board>(rows, cols))
{
}

Game::~Game()
{
}

void Game::passTurn() {
		_currentPlayer = _currentPlayer == 1 ? 2 : 1;
		if (_isLastTurnPass) {
				_isGameOver = true;
				std::cout << "2 pass in a row, the game end!\n";
		}
		_isLastTurnPass = true;
}

void Game::render()
{
    _grid->render();
    Vector2 mousePos = GetMousePosition();
    int row = (int)round((mousePos.x - CELL_SIZE - PADDING) / CELL_SIZE);
    int col = (int)round((mousePos.y - CELL_SIZE - PADDING) / CELL_SIZE);
    if (_grid->isCellValid(row, col, _currentPlayer)) {
        _grid->renderGhostStones(row, col, _currentPlayer);
    }
}

std::vector<std::vector<int>> Game::getGrid() {
		return _grid->getGrid();
}

void Game::loadFromSnapShot(const std::vector<std::vector<int>>& grid,
														const int& currentPlayer,
														const int& scorePlayer1,
														const int& scorePlayer2) {
		_grid->loadGrid(grid);
		_currentPlayer = currentPlayer;
		_scorePlayer1 = scorePlayer1;
		_scorePlayer2 = scorePlayer2;
}

void Game::DetermineTerritories() {
    vector <vector <bool> > visited(_grid->GetNumRows(), vector <bool>(_grid->GetNumCols(), false));
    vector <pair <int, int> > Stones;
    queue <pair <int, int> > q;

    for (int i = 0; i < _grid->GetNumRows(); i++) {
        for (int j = 0; j < _grid->GetNumCols(); j++) {
            if (visited[i][j] || _grid->getValue(i, j) != 0) continue;

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

                    if (_grid->isCellInside(newX, newY) && !visited[newX][newY]) {
                        if (_grid->getValue(newX, newY) == 0) {
                            visited[newX][newY] = true;
                            q.push({newX, newY});
                        }
                        else if (_grid->getValue(newX, newY) == _currentPlayer) {
                            CurrentPlayer = true;
                        }
                        else OpponentPlayer = true;
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
            }
            else if (_currentPlayer == 2) {
                if (CurrentPlayer)
                    _FinalScorePlayer2 += Stones.size();
                else if (OpponentPlayer)
                    _FinalScorePlayer1 += Stones.size();
            }
        }
    }
}

void Game::SelfCapture(int CurrentPlayer, vector<pair<int, int>> Stones, vector<pair<int, int>> EmptySpots) {
    vector <vector <bool> > visited(_grid->GetNumRows(), vector <bool>(_grid->GetNumCols(), false));
    queue <pair <int, int> > q;
    bool isCaptured = true;
    bool OpponentCaptured = false;
    int OpponentPlayer;

    if (CurrentPlayer == 1) OpponentPlayer = 2;
    else OpponentPlayer = 1;

    for (auto [x, y] : Stones) {
        q.push({x, y});
        visited[x][y] = true;
    }

    q.push({EmptySpots[0].first, EmptySpots[0].second});
    visited[EmptySpots[0].first][EmptySpots[0].second] = true;

    vector <vector <bool> > vis(_grid->GetNumRows(), vector <bool>(_grid->GetNumCols(), false));
    vis[EmptySpots[0].first][EmptySpots[0].second] = true;

    for (int i = 0; i < 4; i++) {
        int newX = EmptySpots[0].first + dx[i];
        int newY = EmptySpots[0].second + dy[i];
        bool TempOpponentCaptured = true;
        bool ExistsOpponentStone = false;

        if (_grid->isCellInside(newX, newY) && _grid->getValue(newX, newY) == OpponentPlayer && !vis[newX][newY]) {
            queue <pair <int, int> > Current;
            ExistsOpponentStone = true;

            Current.push({newX, newY});
            vis[newX][newY] = true;

            while (!Current.empty()) {
                auto [x, y] = Current.front();
                Current.pop();

                for (int j = 0; j < 4; j++) {
                    int u = x + dx[j];
                    int v = y + dy[j];

                    if (_grid->isCellInside(u, v)) {
                        if (_grid->getValue(u, v) == OpponentPlayer && !vis[u][v]) {
                            vis[u][v] = true;
                            Current.push({u, v});
                        }
                        else if (_grid->getValue(u, v) == 0 && !vis[u][v]) {
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
            _grid->setValidPlayer1(EmptySpots[0].first, EmptySpots[0].second, 0);
        else if (CurrentPlayer == 2)
            _grid->setValidPlayer2(EmptySpots[0].first, EmptySpots[0].second, 0);
        return;
    }

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (_grid->isCellInside(newX, newY)) {
                if (_grid->getValue(newX, newY) == CurrentPlayer && !visited[newX][newY]) {
                    visited[newX][newY] = true;
                    q.push({newX, newY});
                }
                else if (_grid->getValue(newX, newY) == 0 && !visited[newX][newY]) {
                    isCaptured = false;
                }
            }
        }
    }

    if (isCaptured) {
        if (CurrentPlayer == 1)
            _grid->setValidPlayer1(EmptySpots[0].first, EmptySpots[0].second, 1);
        else if (CurrentPlayer == 2)
            _grid->setValidPlayer2(EmptySpots[0].first, EmptySpots[0].second, 1);
    }
    else {
        if (CurrentPlayer == 1)
            _grid->setValidPlayer1(EmptySpots[0].first, EmptySpots[0].second, 0);
        else if (CurrentPlayer == 2)
            _grid->setValidPlayer2(EmptySpots[0].first, EmptySpots[0].second, 0);
    }
}

void Game::CheckValid(int CurrentPlayer, int OpponentPlayer) {
    vector <vector <bool> > visited(_grid->GetNumRows(), vector <bool>(_grid->GetNumCols(), false));
    vector <pair <int, int> > Stones;
    vector <pair <int, int> > EmptySpots;
    vector <pair <int, int> > OpponentStones;

    for (int i = 0; i < _grid->GetNumRows(); i++) {
        for (int j = 0; j < _grid->GetNumCols(); j++) {
            if (_grid->getValue(i, j) != CurrentPlayer || visited[i][j]) continue;

            queue <pair <int, int> > q;

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

                    if (_grid->isCellInside(newX, newY)) {
                        if (_grid->getValue(newX, newY) == 0) {
                            EmptySpots.push_back({newX, newY});
                        }
                        else if (_grid->getValue(newX, newY) == CurrentPlayer && !visited[newX][newY]) {
                            q.push({newX, newY});
                            visited[newX][newY] = true;
                        }
                    }
                }
            }

            if (EmptySpots.size() == 1 && !_grid->Alive(EmptySpots[0].first, EmptySpots[0].second)) {
                if ((CurrentPlayer == 1 && _grid->getValidPlayer1(EmptySpots[0].first, EmptySpots[0].second) == 0) || (CurrentPlayer == 2 && _grid->getValidPlayer2(EmptySpots[0].first, EmptySpots[0].second) == 0))
                    SelfCapture(CurrentPlayer, Stones, EmptySpots);
            }
        }
    }

    for (int i = 0; i < _grid->GetNumRows(); i++) {
        for (int j = 0; j < _grid->GetNumCols(); j++) {
            if (_grid->getValue(i, j) == 0 && !_grid->Alive(i, j)) {
                if (CurrentPlayer == 1 && _grid->getValidPlayer1(i, j)) continue;
                if (CurrentPlayer == 2 && _grid->getValidPlayer2(i, j)) continue;

                Stones.clear();
                EmptySpots.clear();
                EmptySpots.push_back({i, j});
                SelfCapture(CurrentPlayer, Stones, EmptySpots);
            }
        }
    }
}

void Game::BeingCaptured(int CurrentPlayer, int OpponentPlayer, pair<int, int> lastMove) {
    vector <vector <bool> > visited(_grid->GetNumRows(), vector <bool> (_grid->GetNumCols(), false));
    vector <pair <int, int> > Stones;
    vector <pair <int, int> > OpponentStones;

    for (int i = 0; i < _grid->GetNumRows(); i++) {
        for (int j = 0; j < _grid->GetNumCols(); j++) {
            if (_grid->getValue(i, j) != CurrentPlayer || visited[i][j]) continue;

            queue <pair <int, int> > q;
            bool isCaptured = true;

            q.push({i, j});
            visited[i][j] = true;
            Stones.clear();
            OpponentStones.clear();

            while (!q.empty()) {
                auto [x, y] = q.front();
                Stones.push_back({x, y});
                q.pop();

                if (x == lastMove.first && y == lastMove.second && _grid->getValue(x, y) == CurrentPlayer)
                    isCaptured = false;

                for (int k = 0; k < 4; k++) {
                    int newX = x + dx[k];
                    int newY = y + dy[k];

                    if (_grid->isCellInside(newX, newY)) {
                        if (_grid->getValue(newX, newY) == 0) {
                            isCaptured = false;
                        }
                        else if (_grid->getValue(newX, newY) == CurrentPlayer && !visited[newX][newY]) {
                            visited[newX][newY] = true;
                            q.push({newX, newY});
                        }
                        else if (_grid->getValue(newX, newY) == OpponentPlayer) {
                            OpponentStones.push_back({newX, newY});
                        }
                    }
                }
            }

            if (isCaptured) {
                for (auto [x, y] : Stones) {
                    _grid->setValue(x, y, 0);
                    _grid->setValidPlayer1(x, y, 0);
                    _grid->setValidPlayer2(x, y, 0);
                }

                if (CurrentPlayer == 1) {
                    _scorePlayer2 += Stones.size();
                }
                else if (CurrentPlayer == 2) {
                    _scorePlayer1 += Stones.size();
                }

                if (Stones.size() == 1) {
                    int TotalCurrentPlayer = 0;
                    int TotalInsideCells = 0;

                    for (int i = 0; i < 4; i++) {
                        int newX = lastMove.first + dx[i];
                        int newY = lastMove.second + dy[i];

                        if (_grid->isCellInside(newX, newY)) {
                            if (newX != Stones[0].first && newY != Stones[0].second) {
                                TotalInsideCells++;

                                if (_grid->getValue(newX, newY) == CurrentPlayer)
                                    TotalCurrentPlayer++;
                            }
                        }
                    }

                    if (TotalCurrentPlayer == TotalInsideCells) {
                        if (CurrentPlayer == 1) {
                            _grid->setValidPlayer1(Stones[0].first, Stones[0].second, 2);
                        }
                        else if (CurrentPlayer == 2) {
                            _grid->setValidPlayer2(Stones[0].first, Stones[0].second, 2);
                        }
                    }
                }
            }
            else {
                for (auto [x, y] : Stones) {
                    _grid->setValidPlayer1(x, y, 1);
                    _grid->setValidPlayer2(x, y, 1);
                }
            }
        }
    }
}

bool Game::handleInput()
{
    if (_isGameOver) return false;
    Vector2 mousePos = GetMousePosition();
    bool isMouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    int row = (int)round((mousePos.x - CELL_SIZE - PADDING) / CELL_SIZE);
    int col = (int)round((mousePos.y - CELL_SIZE - PADDING) / CELL_SIZE);
		bool isValidMove = 0;

    if (isMouseClicked && _grid->isCellValid(row, col, _currentPlayer)) {
        _grid->setValue(row, col, _currentPlayer);
        _grid->setValidPlayer1(row, col, 1);
        _grid->setValidPlayer2(row, col, 1);

        for (int i = 0; i < _grid->GetNumRows(); i++) {
            for (int j = 0; j < _grid->GetNumCols(); j++) {
                if (_currentPlayer == 1 && _grid->getValidPlayer2(i, j) == 2) {
                    if (_grid->getValue(i, j) == 0)
                        _grid->setValidPlayer2(i, j, 0);
                    else _grid->setValidPlayer2(i, j, 1);
                }
                else if (_currentPlayer == 3 && _grid->getValidPlayer1(i, j) == 2) {
                    if (_grid->getValue(i, j) == 0)
                        _grid->setValidPlayer1(i, j, 0);
                    else _grid->setValidPlayer1(i, j, 1);
                }
            }
        }

        _currentPlayer = ((_currentPlayer == 1) ? 2 : 1);
				isValidMove = true;
    }

    //Checking for captures

    BeingCaptured(1, 2, {row, col}); //Check for Black Stones being captured by White
    BeingCaptured(2, 1, {row, col}); //Check for White Stones being captured by Black
    CheckValid(1, 2); //Check for valid moves for Black
    CheckValid(2, 1); //Check for valid moves for White

    //When the Game is Over

    if (_isGameOver) {
        DetermineTerritories();
        _scorePlayer1 += _FinalScorePlayer1;
        _scorePlayer2 += _FinalScorePlayer2;
    }
		if (isValidMove) _isLastTurnPass = false;
		return isValidMove;
}
