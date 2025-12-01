#pragma once
#include "raylib.h"
#include "SettingsData.h"
#include <string>
#include <vector>

class ThemeController {
public:
    ThemeController();
    virtual ~ThemeController() = default;

    virtual void render(Vector2 position, Vector2 size) const;
    virtual void handleInput(Vector2 position, Vector2 size);

    void setTheme(ThemeType theme);
    ThemeType getTheme() const;
    void setColor(int colorIndex);
    int getColor() const;

    // Available colors for selection
    static const std::vector<Color>& getColorPalette();

protected:
    ThemeType _theme = ThemeType::Classic;
    int _colorIndex = 0;

    // UI constants
    static constexpr float FONT_SIZE = 20.0f;
    static constexpr float CONTENT_TOP_PADDING = 40.0f;
    static constexpr float THEME_BOX_WIDTH = 90.0f;
    static constexpr float THEME_BOX_HEIGHT = 30.0f;
    static constexpr float COLOR_SWATCH_SIZE = 25.0f;
    static constexpr float PADDING = 10.0f;
    static constexpr int COLORS_PER_ROW = 8;
    static constexpr float PREVIEW_X_OFFSET = 400.0f;
    static constexpr float PREVIEW_SIZE = 100.0f;
    static constexpr float PREVIEW_IMAGE_SCALE = 0.05f;

    // Draw functions
    void drawThemeSelection(Vector2 position, Vector2 size) const;
    void drawColorSelection(Vector2 position, Vector2 size) const;
    void drawPreview(Vector2 position, Vector2 size) const;

    // Interaction functions
    int getClickedTheme(Vector2 mousePos, Vector2 position) const;
    int getClickedColor(Vector2 mousePos, Vector2 position, Vector2 size) const;

    // Override these in derived classes
    virtual void applyTheme();
    virtual void applyColor();
    virtual void syncFromSettings();

    // Theme names
    static const char* getThemeName(ThemeType theme);
};
