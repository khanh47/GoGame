// src/Render/Settings/BoardSizeController.cpp
#include "BoardSizeController.h"
#include "ResourceManager.h"
#include "SettingsData.h"
#include <string>

BoardSizeController::BoardSizeController() {
    syncFromSettings();
}

void BoardSizeController::syncFromSettings() {
    _boardSize = SettingsData::getInstance().getBoardSize();
}

void BoardSizeController::applyBoardSize() {
    SettingsData::getInstance().setBoardSize(_boardSize);
}

void BoardSizeController::setBoardSize(int size) {
    _boardSize = size;
    applyBoardSize();
}

void BoardSizeController::render(Vector2 position, Vector2 size) const {
    Font font = ResourceManager::getInstance().getFont("GozaruDemo");
    
    // Draw "Board Size:" label
    DrawTextEx(font, "Board Size:", {position.x + PADDING, position.y + 10}, FONT_SIZE, 1.0f, BLACK);
    
    // Calculate starting position for size buttons
    float startX = position. x + PADDING + 100;
    float buttonY = position.y + 5;
    
    for (size_t i = 0; i < BOARD_SIZES.size(); i++) {
        float buttonX = startX + i * (BUTTON_WIDTH + BUTTON_SPACING);
        
        bool isSelected = (_boardSize == BOARD_SIZES[i]);
        
        // Colors
        Color bgColor = isSelected ? Color{100, 149, 237, 255} : Color{220, 220, 220, 255};
        Color textColor = isSelected ? WHITE : BLACK;
        
        // Hover effect
        Vector2 mousePos = GetMousePosition();
        Rectangle buttonRect = {buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT};
        if (CheckCollisionPointRec(mousePos, buttonRect) && !isSelected) {
            bgColor = Color{180, 180, 180, 255};
        }
        
        // Draw button
        DrawRectangle((int)buttonX, (int)buttonY, (int)BUTTON_WIDTH, (int)BUTTON_HEIGHT, bgColor);
        DrawRectangleLinesEx(buttonRect, 2.0f, DARKGRAY);
        
        // Draw size text (e.g., "9x9", "13x13", "19x19")
        char sizeText[16];
        snprintf(sizeText, sizeof(sizeText), "%dx%d", BOARD_SIZES[i], BOARD_SIZES[i]);
        
        Vector2 textSize = MeasureTextEx(font, sizeText, FONT_SIZE, 1.0f);
        float textX = buttonX + (BUTTON_WIDTH - textSize.x) / 2;
        float textY = buttonY + (BUTTON_HEIGHT - textSize.y) / 2;
        DrawTextEx(font, sizeText, {textX, textY}, FONT_SIZE, 1.0f, textColor);
    }
}

void BoardSizeController::handleInput(Vector2 position, Vector2 size) {
    if (! IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;
    
    Vector2 mousePos = GetMousePosition();
    int clickedSize = getClickedSize(mousePos, position);
    
    if (clickedSize > 0) {
        setBoardSize(clickedSize);
    }
}

int BoardSizeController::getClickedSize(Vector2 mousePos, Vector2 position) const {
    float startX = position.x + PADDING + 100;
    float buttonY = position.y + 5;
    
    for (size_t i = 0; i < BOARD_SIZES. size(); i++) {
        float buttonX = startX + i * (BUTTON_WIDTH + BUTTON_SPACING);
        Rectangle buttonRect = {buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT};
        
        if (CheckCollisionPointRec(mousePos, buttonRect)) {
            return BOARD_SIZES[i];
        }
    }
    
    return -1;
}