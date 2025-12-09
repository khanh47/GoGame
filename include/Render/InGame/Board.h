#pragma once
#include <vector>

extern const float PADDING;

class Board {
public:
	Board(int rows, int cols);
	~Board();
	void init();
	void reset();
	void render();
	void renderGhostStones(int row, int col, int value);
	void setValue(int row, int col, int value);
	int getValue(int row, int col);
	int GetNumRows() { return numRows; }
	int GetNumCols() { return numCols; }
	bool isCellInside(int row, int col);
	std::vector<std::vector<int>> getGrid() { return _grid; }
	void setGrid(const std::vector<std::vector<int>> &grid);
    float getCellSize() const { return _cellSize; }
	std::vector<std::pair<int, int>> getStarPoints() const;
	void renderStarPoints();


private:
	int numRows;
	int numCols;
    float _cellSize;
	std::vector<std::vector<int>> _grid;
};
