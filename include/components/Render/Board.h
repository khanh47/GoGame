#pragma once
#include <vector>

extern const float CELL_SIZE;
extern const float PADDING;

class Board {
public:
    Board(int rows, int cols);
    ~Board();
    void render();
    void renderBorderLines();
    void renderBoard();
    void renderStones();
    void renderGhostStones(int row, int col, int value);
    void setValue(int row, int col, int value);
    int getValue(int row, int col);
    bool isCellInside(int row, int col);

private:
    int numRows;
    int numCols;
    std::vector<std::vector<int>> grid;
};