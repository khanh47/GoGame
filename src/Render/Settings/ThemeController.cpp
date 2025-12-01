#include "ThemeController.h"
#include "ResourceManager.h"
#include <algorithm>
#include <cmath>
#include "colors.h"

// Static color palette
const std::vector<Color>& ThemeController::getColorPalette() {
    static std::vector<Color> palette = {
        {0, 0, 0, 255},         // Black
        {64, 64, 64, 255},      // Dark Gray
        {128, 128, 128, 255},   // Gray
        {192, 192, 192, 255},   // Light Gray
        {255, 255, 255, 255},   // White
        {210, 180, 140, 255},   // Light Brown
        {210, 105, 30, 255},    // Chocolate
        {222, 184, 135, 255},   // Burlywood
        
        {255, 0, 0, 255},       // Red
        {255, 99, 71, 255},     // Tomato
        {255, 165, 0, 255},     // Orange
        {255, 215, 0, 255},     // Gold
        {255, 255, 0, 255},     // Yellow
        {154, 205, 50, 255},    // Yellow Green
        {0, 128, 0, 255},       // Green
        {0, 100, 0, 255},       // Dark Green
        
        {0, 128, 128, 255},     // Teal
        {64, 224, 208, 255},    // Turquoise
        {0, 191, 255, 255},     // Deep Sky Blue
        {0, 0, 255, 255},       // Blue
        {0, 0, 139, 255},       // Dark Blue
        {128, 0, 128, 255},     // Purple
        {255, 20, 147, 255},    // Deep Pink
        {255, 182, 193, 255}    // Light Pink
    };
    return palette;
}

static void DrawTextureCenteredAtDiameter(const Texture2D &tex, float cx, float cy, float targetDiam) {
    if (tex.id == 0) return;

    Rectangle src = { 0.f, 0.f, (float)tex.width, (float)tex.height };

    float dstX = std::round(cx - targetDiam * 0.5f);
    float dstY = std::round(cy - targetDiam * 0.5f);
    Rectangle dst = { dstX, dstY, targetDiam, targetDiam };

    Vector2 origin = { 0.f, 0.f };

    DrawTexturePro(tex, src, dst, origin, 0.0f, WHITE);
}

ThemeController::ThemeController() {
    syncFromSettings();
}

void ThemeController::render(Vector2 position, Vector2 size) const {
    position.y += CONTENT_TOP_PADDING;
    drawThemeSelection(position, size);
    
    // Color selection starts below theme selection
    Vector2 colorPos = {position.x, position.y + THEME_BOX_HEIGHT + PADDING * 2};
    Vector2 colorSize = {size.x, size.y - THEME_BOX_HEIGHT - PADDING * 2};
    drawColorSelection(colorPos, colorSize);
    
    // Preview image to the right of the selections
    Vector2 previewPos = {position.x + PREVIEW_X_OFFSET, position.y};
    Vector2 previewSize = {PREVIEW_SIZE, PREVIEW_SIZE};
    drawPreview(previewPos, previewSize);
}

void ThemeController::handleInput(Vector2 position, Vector2 size) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;
    position.y += CONTENT_TOP_PADDING;
    
    Vector2 mousePos = GetMousePosition();

    // Check theme selection
    int clickedTheme = getClickedTheme(mousePos, position);
    if (clickedTheme >= 0) {
        setTheme(static_cast<ThemeType>(clickedTheme));
        return;
    }

    // Check color selection
    Vector2 colorPos = {position.x, position.y + THEME_BOX_HEIGHT + PADDING * 2};
    Vector2 colorSize = {size.x, size.y - THEME_BOX_HEIGHT - PADDING * 2};
    int clickedColor = getClickedColor(mousePos, colorPos, colorSize);
    if (clickedColor >= 0) {
        setColor(clickedColor);
    }
}

void ThemeController::setTheme(ThemeType theme) {
    _theme = theme;
    applyTheme();
}

ThemeType ThemeController::getTheme() const {
    return _theme;
}

void ThemeController::setColor(int colorIndex) {
    _colorIndex = colorIndex;
    applyColor();
}

int ThemeController::getColor() const {
    return _colorIndex;
}

void ThemeController::drawThemeSelection(Vector2 position, Vector2 size) const {
    Font font = ResourceManager::getInstance().getFont("GozaruDemo");
    
    // Draw "Theme:" label
    DrawTextEx(font, "Theme", {position.x + PADDING, position.y + 5}, FONT_SIZE, 1.0f, BLACK);
    
    // Calculate starting position for theme boxes
    float startX = position.x + PADDING + 70;
    float totalWidth = 3 * THEME_BOX_WIDTH + 2 * PADDING;
    
    ThemeType themes[] = {ThemeType::Classic, ThemeType::Cartoon, ThemeType::Aesthetic};
    
    for (int i = 0; i < 3; i++) {
        float boxX = startX + i * (THEME_BOX_WIDTH + PADDING);
        float boxY = position.y;
        
        bool isSelected = (_theme == themes[i]);
        Color bgColor = isSelected ? Color{100, 149, 237, 255} : Color{220, 220, 220, 255};
        Color textColor = isSelected ? WHITE : BLACK;
        
        DrawRectangle((int)boxX, (int)boxY, (int)THEME_BOX_WIDTH, (int)THEME_BOX_HEIGHT, bgColor);
        DrawRectangleLinesEx({boxX, boxY, THEME_BOX_WIDTH, THEME_BOX_HEIGHT}, 2.0f, DARKGRAY);
        
        const char* themeName = getThemeName(themes[i]);
        Vector2 textSize = MeasureTextEx(font, themeName, 14.0f, 1.0f);
        float textX = boxX + (THEME_BOX_WIDTH - textSize.x) / 2;
        float textY = boxY + (THEME_BOX_HEIGHT - textSize.y) / 2;
        DrawTextEx(font, themeName, {textX, textY}, 16.0f, 1.0f, textColor);
    }
}

void ThemeController::drawColorSelection(Vector2 position, Vector2 size) const {
    Font font = ResourceManager::getInstance().getFont("GozaruDemo");
    
    // Draw "Color:" label
    DrawTextEx(font, "Color:", {position.x + PADDING, position.y + 5}, FONT_SIZE, 1.0f, BLACK);
    
    const auto& palette = getColorPalette();
    float startX = position.x + PADDING + 70;
    float startY = position.y;
    
    for (size_t i = 0; i < palette.size(); i++) {
        int row = i / COLORS_PER_ROW;
        int col = i % COLORS_PER_ROW;
        
        float swatchX = startX + col * (COLOR_SWATCH_SIZE + 5);
        float swatchY = startY + row * (COLOR_SWATCH_SIZE + 5);
        
        bool isSelected = (static_cast<int>(i) == _colorIndex);
        
        // Draw selection border
        if (isSelected) {
            DrawRectangle((int)(swatchX - 2), (int)(swatchY - 2), 
                          (int)(COLOR_SWATCH_SIZE + 4), (int)(COLOR_SWATCH_SIZE + 4), 
                          Color{100, 149, 237, 255});
        }
        
        DrawRectangle((int)swatchX, (int)swatchY, 
                      (int)COLOR_SWATCH_SIZE, (int)COLOR_SWATCH_SIZE, palette[i]);
        DrawRectangleLinesEx({swatchX, swatchY, COLOR_SWATCH_SIZE, COLOR_SWATCH_SIZE}, 
                             1.0f, DARKGRAY);
    }
}

int ThemeController::getClickedTheme(Vector2 mousePos, Vector2 position) const {
    float startX = position.x + PADDING + 70;
    
    for (int i = 0; i < 3; i++) {
        float boxX = startX + i * (THEME_BOX_WIDTH + PADDING);
        float boxY = position.y;
        
        Rectangle box = {boxX, boxY, THEME_BOX_WIDTH, THEME_BOX_HEIGHT};
        if (CheckCollisionPointRec(mousePos, box)) {
            return i;
        }
    }
    return -1;
}

int ThemeController::getClickedColor(Vector2 mousePos, Vector2 position, Vector2 size) const {
    const auto& palette = getColorPalette();
    float startX = position.x + PADDING + 70;
    float startY = position.y;
    
    for (size_t i = 0; i < palette.size(); i++) {
        int row = i / COLORS_PER_ROW;
        int col = i % COLORS_PER_ROW;
        
        float swatchX = startX + col * (COLOR_SWATCH_SIZE + 5);
        float swatchY = startY + row * (COLOR_SWATCH_SIZE + 5);
        
        Rectangle swatch = {swatchX, swatchY, COLOR_SWATCH_SIZE, COLOR_SWATCH_SIZE};
        if (CheckCollisionPointRec(mousePos, swatch)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void ThemeController::applyTheme() {
    // Base implementation does nothing - override in derived classes
}

void ThemeController::applyColor() {
    // Base implementation does nothing - override in derived classes
}

void ThemeController::syncFromSettings() {
    // Base implementation does nothing - override in derived classes
}

const char* ThemeController::getThemeName(ThemeType theme) {
    switch (theme) {
        case ThemeType::Classic: return "Classic";
        case ThemeType::Cartoon: return "Cartoon";
        case ThemeType::Aesthetic: return "Aesthetic";
        default: return "Unknown";
    }
}

void ThemeController::drawPreview(Vector2 position, Vector2 size) const {
    Font font = ResourceManager::getInstance().getFont("GozaruDemo");
    
    // Draw preview label
    DrawTextEx(font, "Preview:", {position.x, position.y - 20}, FONT_SIZE, 1.0f, BLACK);
    
    // Draw preview background (represents board color)
    const auto& palette = getColorPalette();
    Color previewColor = (_colorIndex >= 0 && _colorIndex < static_cast<int>(palette.size())) 
                         ? palette[_colorIndex] 
                         : Color{222, 184, 135, 255}; // Default burlywood
    
    // Draw preview box with selected color
    DrawRectangle((int)position.x, (int)position.y, (int)size.x, (int)size.y, previewColor);
    DrawRectangleLinesEx({position.x, position.y, size.x, size.y}, 2.0f, DARKGRAY);
    
    // Draw a mini grid pattern to simulate a Go board
    float gridSpacing = size.x / 4.0f;
    Color gridColor = {0, 0, 0, 100};  // Semi-transparent black for grid lines
    
    for (int i = 1; i < 4; i++) {
        // Vertical lines
        float x = position.x + i * gridSpacing;
        DrawLine((int)x, (int)position.y, (int)x, (int)(position.y + size.y), gridColor);
        // Horizontal lines
        float y = position.y + i * gridSpacing;
        DrawLine((int)position.x, (int)y, (int)(position.x + size.x), (int)y, gridColor);
    }
    
    // Draw sample stones based on theme type
    float stoneRadius = gridSpacing * 0.35f;
    float centerX1 = position.x + gridSpacing;
    float centerY1 = position.y + gridSpacing;
    float centerX2 = position.x + 3 * gridSpacing;
    float centerY2 = position.y + 3 * gridSpacing;
    float targetDiam = stoneRadius * 2.0f;
    
    // Draw black stone
    DrawCircleV(Vector2{centerX1 + 2, centerY1 + 2}, stoneRadius, shadow);
    if (_theme == ThemeType::Cartoon) {
        Texture2D &tex = ResourceManager::getInstance().getTexture2D("black_cartoon_stone");
        DrawTextureCenteredAtDiameter(tex, centerX1, centerY1, targetDiam * 1.15f);
    } else if (_theme == ThemeType::Aesthetic) {
        Texture2D &tex = ResourceManager::getInstance().getTexture2D("black_aesthetic_stone");
        DrawTextureCenteredAtDiameter(tex, centerX1, centerY1, targetDiam);
    } else {
        DrawCircle(centerX1, centerY1, stoneRadius, BLACK);
    }
    
    // Draw white stone
    DrawCircleV(Vector2{centerX2 + 2, centerY2 + 2}, stoneRadius, shadow);
    if (_theme == ThemeType::Cartoon) {
        Texture2D &tex = ResourceManager::getInstance().getTexture2D("white_cartoon_stone");
        DrawTextureCenteredAtDiameter(tex, centerX2, centerY2, targetDiam * 1.15f);
    } else if (_theme == ThemeType::Aesthetic) {
        Texture2D &tex = ResourceManager::getInstance().getTexture2D("white_aesthetic_stone");
        DrawTextureCenteredAtDiameter(tex, centerX2, centerY2, targetDiam);
    } else {
        DrawCircle(centerX2, centerY2, stoneRadius, WHITE);
    }
}
