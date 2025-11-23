#include "Board.h"
#include "raylib.h"
#include "colors.h"
#include <cmath>
#include <iostream>

const float CELL_SIZE = 36;
const float PADDING = 50;

Board::Board(int rows, int cols) : numRows(rows), numCols(cols) {
    init();
}

Board::~Board()
{
}

void Board::init()
{
    _grid.resize(numRows, std::vector<int>(numCols, 0));
    validPlayer1.resize(numRows, std::vector<int>(numCols, 0));
    validPlayer2.resize(numRows, std::vector<int>(numCols, 0));
}

void Board::renderBorderLines()
{
    Rectangle recBorderLines = {PADDING, PADDING, (numCols + 1) * CELL_SIZE, (numRows + 1) * CELL_SIZE};
    DrawRectangleLinesEx(recBorderLines, 1, BLACK);
}

void Board::renderBoard()
{
    DrawRectangle(PADDING, PADDING, (numCols + 1) * CELL_SIZE, (numRows + 1) * CELL_SIZE, lightBrown);
    for (int i = 1; i <= numRows; i++)
    {
        DrawLine(PADDING + CELL_SIZE, PADDING + i * CELL_SIZE, PADDING + numRows * CELL_SIZE, PADDING + i * CELL_SIZE, BLACK);
    }
    for (int i = 1; i <= numCols; i++)
    {
        DrawLine(PADDING + i * CELL_SIZE, PADDING + CELL_SIZE, PADDING + i * CELL_SIZE, PADDING + numRows * CELL_SIZE, BLACK);
    }
}

void Board::renderStones()
{
    float radius = CELL_SIZE * 0.4f;
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            float x = PADDING + (i + 1) * CELL_SIZE;
            float y = PADDING + (j + 1) * CELL_SIZE;
            if (_grid[i][j])
            {
                DrawCircleV(Vector2{x + 2, y + 2}, radius, shadow);
                DrawCircle(x, y, radius, _grid[i][j] == 1 ? WHITE : BLACK);
            }
        }
    }
}

void Board::renderGhostStones(int row, int col, int value)
{
    float radius = CELL_SIZE * 0.4f;
    DrawCircle(PADDING + (row + 1) * CELL_SIZE, PADDING + (col + 1) * CELL_SIZE, radius, value == 1 ? ghostWhite : shadow);
}

void Board::render()
{
    renderBorderLines();
    renderBoard();
    renderStones();
}

void Board::setValue(int row, int col, int value)
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        _grid[row][col] = value;
    }
}

int Board::getValue(int row, int col)
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        return _grid[row][col];
    }
    return -1;
}

bool Board::isCellInside(int row, int col)
{
    return row >= 0 && row < numRows && col >= 0 && col < numCols;
}

bool Board::isCellValid(int row, int col, int CurrentPlayer) {
    return (isCellInside(row, col) && ((CurrentPlayer == 1 && validPlayer1[row][col] == 0) || (CurrentPlayer == 2 && validPlayer2[row][col] == 0)));
}

std::vector<std::vector<int>> Board::getGrid() {
		return _grid;
}

void Board::loadGrid(const std::vector<std::vector<int>>& grid) {
		_grid = grid;
}

void Board::setValidPlayer1(int row, int col, int value)
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        validPlayer1[row][col] = value;
    }
}

void Board::setValidPlayer2(int row, int col, int value)
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        validPlayer2[row][col] = value;
    }
}

int Board::getValidPlayer1(int row, int col)
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        return validPlayer1[row][col];
    }
    return -1;
}

int Board::getValidPlayer2(int row, int col)
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        return validPlayer2[row][col];
    }
    return -1;
}

int Board::GetNumRows()
{
    return numRows;
}

int Board::GetNumCols()
{
    return numCols;
}

bool Board::Alive(int row, int col)
{
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};

    for (int i = 0; i < 4; i++) {
        int newX = row + dx[i];
        int newY = col + dy[i];

        if (isCellInside(newX, newY) && _grid[newX][newY] == 0) {
            return true;
        }
    }
    return false;
}
