#include "Board.h"
#include "raylib.h"
#include "colors.h"
#include <cmath>

const float CELL_SIZE = 36;
const float PADDING = 36;

Board::Board(int rows, int cols) : numRows(rows), numCols(cols) {
    init();
}

Board::~Board()
{
}

void Board::init()
{
    grid.resize(numRows, std::vector<int>(numCols, 0));
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
            if (grid[i][j])
            {   
                DrawCircleV(Vector2{x + 2, y + 2}, radius, shadow);
                DrawCircle(x, y, radius, grid[i][j] == 1 ? WHITE : BLACK);
            }
        }
    }
}

void Board::renderGhostStones(int row, int col, int value)
{
    float radius = CELL_SIZE * 0.4;
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
        grid[row][col] = value;
    }
}

int Board::getValue(int row, int col)
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        return grid[row][col];
    }
    return -1;
}

bool Board::isCellInside(int row, int col)
{
    return row >= 0 && row < numRows && col >= 0 && col < numCols;
}
