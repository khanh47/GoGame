#pragma once
#include <vector>

extern const float CELL_SIZE;
extern const float PADDING;

class Board {
public:
    Board(int rows, int cols);
    ~Board();
    void init();
    void render();
    void renderBorderLines();
    void renderBoard();
    void renderStones();
    void renderGhostStones(int row, int col, int value);
    void setValue(int row, int col, int value);
    int getValue(int row, int col);
    void setValidPlayer1(int row, int col, int value);
    void setValidPlayer2(int row, int col, int value);
    int getValidPlayer1(int row, int col);
    int getValidPlayer2(int row, int col);
    int GetNumRows();
    int GetNumCols();
    bool Alive(int row, int col);
    bool isCellInside(int row, int col);
    bool isCellValid(int row, int col, int CurrentPlayer);
		std::vector<std::vector<int>> getGrid();
		void loadGrid(const std::vector<std::vector<int>>& grid);

private:
    int numRows;
    int numCols;
    std::vector<std::vector<int>> _grid;
    std::vector<std::vector<int>> validPlayer1;
    std::vector<std::vector<int>> validPlayer2;
};
