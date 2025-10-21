#include "Board.h"
#include "raylib.h"
#include "colors.h"
#include <cmath>

Board::Board(int rows, int cols)
{
    numRows = rows;
    numCols = cols;
    grid.resize(rows, std::vector<int>(cols, 0));
    cellSize = 36;   
}

Board::~Board()
{
}

void Board::Draw()
{
    const int padding = 36;
    Rectangle recBorderLines = {(float) padding, (float) padding, (float) (numCols + 1) * cellSize, (float) (numRows + 1) * cellSize};
    DrawRectangle(padding, padding, (numCols + 1) * cellSize, (numRows + 1) * cellSize, lightBrown);
    DrawRectangleLinesEx(recBorderLines, 1, BLACK);
    for (int i = 1; i <= numRows; i++)
    {
        DrawLine(padding + cellSize, padding + i * cellSize, padding + numRows * cellSize, padding + i * cellSize, BLACK);
    }
    for (int i = 1; i <= numCols; i++)
    {
        DrawLine(padding + i * cellSize, padding + cellSize, padding + i * cellSize, padding + numRows * cellSize, BLACK);
    }
    float radius = cellSize * 0.4f;
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            float x = padding + (i + 1) * cellSize;
            float y = padding + (j + 1) * cellSize;
            if (grid[i][j])
            {   
                DrawCircleV(Vector2{x + 2, y + 2}, radius, {0, 0, 0, 100});
                DrawCircle(x, y, radius, grid[i][j] == 1 ? WHITE : BLACK);
            }
        }
    }
}

void Board::SetValue(int row, int col, int value)
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        grid[row][col] = value;
    }
}

int Board::GetValue(int row, int col)
{
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        return grid[row][col];
    }
    return -1;
}

bool Board::IsCellInside(int row, int col)
{
    return row >= 0 && row < numRows && col >= 0 && col < numCols;
}
