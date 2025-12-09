#pragma once

#include "raylib.h"
#include <vector>

class BoardSizeController {
public:
    BoardSizeController();
    
    void render(Vector2 position, Vector2 size) const;
    void handleInput(Vector2 position, Vector2 size);
    
    int getBoardSize() const { return _boardSize; }
    void setBoardSize(int size);

private:
    void syncFromSettings();
    void applyBoardSize();
    int getClickedSize(Vector2 mousePos, Vector2 position) const;
    
    int _boardSize = 19;
    
    // UI Constants
    static constexpr float PADDING = 10.0f;
    static constexpr float BUTTON_WIDTH = 80.0f;
    static constexpr float BUTTON_HEIGHT = 40.0f;
    static constexpr float BUTTON_SPACING = 15.0f;
    static constexpr float FONT_SIZE = 20.0f;
    
    // Available board sizes
    const std::vector<int> BOARD_SIZES = {9, 13, 19};
};