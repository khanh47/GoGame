#pragma once
#include <vector>

class Board {
public:
    Board(int rows, int cols);
    ~Board();
    std::vector<std::vector<int>> grid;
    void Draw();
    void SetValue(int row, int col, int value);
    int GetValue(int row, int col);

private:
    int numRows;
    int numCols;
    int cellSize;
};