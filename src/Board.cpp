#include "Board.h"
#include "raylib.h"
#include <cmath>

Board::Board(int rows, int cols)
{
    numRows = rows;
    numCols = cols;
    grid.resize(rows, std::vector<int>(cols, 0));
}

Board::~Board()
{
}

void Board::Draw()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    float targetZoneWidth = screenWidth * 0.7f;
    float targetZoneHeight = screenHeight * 0.8f;

    float cellSizeBasedOnWidth = targetZoneWidth / (numCols - 1);
    float cellSizeBasedOnHeight = targetZoneHeight / (numRows - 1);

    int cellSize = (int)fmin(cellSizeBasedOnWidth, cellSizeBasedOnHeight);

    float boardWidth = (numCols - 1) * cellSize;
    float boardHeight = (numRows - 1) * cellSize;

    float startX = (targetZoneWidth - boardWidth) / 2.0f; 
    float startY = (screenHeight - boardHeight) / 2.0f;

    DrawRectangle(startX - cellSize, startY - cellSize, boardWidth + cellSize * 2, boardHeight + cellSize * 2, Color{ 210, 180, 140, 255 });
    DrawRectangleLinesEx(Rectangle{startX - cellSize, startY - cellSize, boardWidth + cellSize * 2, boardHeight + cellSize * 2}, 1.0f, BLACK);

    for (int i = 0; i < numRows; i++) {
        DrawLine(
            startX,
            startY + i * cellSize,
            startX + boardWidth,
            startY + i * cellSize,
            BLACK
        );
    }

    for (int j = 0; j < numCols; j++) {
        DrawLine(
            startX + j * cellSize,
            startY,
            startX + j * cellSize,
            startY + boardHeight,
            BLACK
        );
    }

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (grid[i][j] == 1) {
                DrawCircle(
                    startX + j * cellSize,
                    startY + i * cellSize,
                    cellSize * 0.4f,
                    BLACK
                );
            }
            else if (grid[i][j] == 2) {
                DrawCircle(
                    startX + j * cellSize,
                    startY + i * cellSize,
                    cellSize * 0.4f,
                    WHITE
                );
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
