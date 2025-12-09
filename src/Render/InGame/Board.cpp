#include "Board.h"
#include "ResourceManager.h"
#include "SettingsData.h"
#include "ThemeController.h"
#include "colors.h"
#include "raylib.h"
#include <cmath>
#include <string>

const float PADDING = 58;

static void DrawTextureCenteredAtDiameter(const Texture2D &tex, float cx, float cy, float targetDiam) {
	if (tex.id == 0)
		return;

	Rectangle src = {0.f, 0.f, (float)tex.width, (float)tex.height};

	float dstX = std::round(cx - targetDiam * 0.5f);
	float dstY = std::round(cy - targetDiam * 0.5f);
	Rectangle dst = {dstX, dstY, targetDiam, targetDiam};

	Vector2 origin = {0.f, 0.f};

	DrawTexturePro(tex, src, dst, origin, 0.0f, WHITE);
}

Board::Board(int rows, int cols) : numRows(rows), numCols(cols) { init(); }

Board::~Board() {}

void Board::init() {
	_grid.resize(numRows, std::vector<int>(numCols, 0));
    const float BOARD_TOTAL_SIZE = 740.0f;
    
    _cellSize = BOARD_TOTAL_SIZE / (numRows + 2);
}

void Board::reset() {
	_grid.assign(numRows, std::vector<int>(numCols, 0));
}

void Board::renderGhostStones(int row, int col, int value) {
	float radius = _cellSize * 0.4f;
	DrawCircle(PADDING + (row + 1) * _cellSize, PADDING + (col + 1) * _cellSize, radius,
						 value == 2 ? ghostWhite : shadow);
}

std::vector<std::pair<int, int>> Board::getStarPoints() const {
    std::vector<std::pair<int, int>> points;

    if (numRows == 19 && numCols == 19) {
        points = {
            {3, 3},   {3, 9},   {3, 15},
            {9, 3},   {9, 9},   {9, 15},
            {15, 3},  {15, 9},  {15, 15}
        };
    }
    else if (numRows == 13 && numCols == 13) {
        points = {
            {3, 3},   {3, 9},
                  {6, 6},
            {9, 3},   {9, 9}
        };
    }
    else if (numRows == 9 && numCols == 9) {
        points = {
            {2, 2},   {2, 6},
                  {4, 4},
            {6, 2},   {6, 6}
        };
    }

    return points;
}

void Board::renderStarPoints() {
    auto starPoints = getStarPoints();
    float radius = _cellSize * 0.1f;

    for (auto [row, col] : starPoints) {
        float x = PADDING + (row + 1) * _cellSize;
        float y = PADDING + (col + 1) * _cellSize;
        DrawCircle(x, y, radius, BLACK);
    }
}

void Board::render() {
	// Render Border Lines
	Rectangle recBorderLines = {PADDING, PADDING, (numCols + 1) * _cellSize, (numRows + 1) * _cellSize};
	DrawRectangleLinesEx(recBorderLines, 1, BLACK);
	Color boardColor = SettingsData::getInstance().getBoardColorAsColor();

	// Render Board
	DrawRectangle(PADDING, PADDING, (numCols + 1) * _cellSize, (numRows + 1) * _cellSize, boardColor);
	float lineThickness = 2.0f;  // Adjust this!  (1.0 = thin, 2-3 = medium, 4+ = thick)

	// Horizontal lines
	for (int i = 1; i <= numRows; i++) {
		Vector2 start = {PADDING + _cellSize, PADDING + i * _cellSize};
		Vector2 end = {PADDING + numCols * _cellSize, PADDING + i * _cellSize};
		DrawLineEx(start, end, lineThickness, BLACK);
	}

	// Vertical lines
	for (int i = 1; i <= numCols; i++) {
		Vector2 start = {PADDING + i * _cellSize, PADDING + _cellSize};
		Vector2 end = {PADDING + i * _cellSize, PADDING + numRows * _cellSize};
		DrawLineEx(start, end, lineThickness, BLACK);
	}
	
	// Draw star points (Hoshi)
	renderStarPoints();

	// Render Stones
	ThemeType stoneTheme = SettingsData::getInstance().getStoneTheme();

	float radius = _cellSize * 0.4f;
	float targetDiam = radius * 2.0f;

	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			float x = PADDING + (i + 1) * _cellSize;
			float y = PADDING + (j + 1) * _cellSize;
			if (_grid[i][j]) {
				DrawCircleV(Vector2{x + 2, y + 2}, radius, shadow);
				std::string alias = _grid[i][j] == 2 ? "white_" : "black_";

				if (stoneTheme == ThemeType::Cartoon) {
					alias += "cartoon_stone";
					Texture2D &tex = ResourceManager::getInstance().getTexture2D(alias);
					DrawTextureCenteredAtDiameter(tex, x, y, targetDiam * 1.15f);
				} else if (stoneTheme == ThemeType::Aesthetic) {
					alias += "aesthetic_stone";
					Texture2D &tex = ResourceManager::getInstance().getTexture2D(alias);
					DrawTextureCenteredAtDiameter(tex, x, y, targetDiam);
				} else {
					DrawCircle(x, y, radius, _grid[i][j] == 2 ? WHITE : BLACK);
				}
			}
		}
	}
}

void Board::setValue(int row, int col, int value) {
	if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
		_grid[row][col] = value;
	}
}

int Board::getValue(int row, int col) {
	if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
		return _grid[row][col];
	}
	return -1;
}

bool Board::isCellInside(int row, int col) { return row >= 0 && row < numRows && col >= 0 && col < numCols; }

void Board::setGrid(const std::vector<std::vector<int>> &grid) { _grid = grid; }
