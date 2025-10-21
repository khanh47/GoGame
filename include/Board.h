#pragma once
#include "position.h"
#include <vector>

class Board {
public:
    Board(int rows, int cols);
    ~Board();
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<Position>> pos;
    void Draw();
    void SetValue(int row, int col, int value);
    int GetValue(int row, int col);
    bool IsCellInside(int row, int col);

private:
    int numRows;
    int numCols;
    int cellSize;
};