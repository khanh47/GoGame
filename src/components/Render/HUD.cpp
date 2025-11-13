#include "HUD.h"
#include "ResourceManager.h"
#include "raylib.h"
#include <cmath>

HUD::HUD()
{
}

void HUD::render()
{
    Texture2D& _infoPanel = ResourceManager::getInstance().getTexture2D("info_panel");
    DrawTextureEx(_infoPanel, {750, 35}, 0.0f, 2.5f, WHITE);
    
    // Draw players info
    Font& font = ResourceManager::getInstance().getFont("GozaruDemo");
    Texture2D& _ready = ResourceManager::getInstance().getTexture2D("ready_image");

    float fontSize = 40.0f;
    float spacing = 1.0f;
    float radius = 25.0f;

    if (_currentPlayer == 1) {
        DrawTextureEx(_ready, {1100, 120}, 0.0f, 0.1f, WHITE);
    } else {
        DrawTextureEx(_ready, {1100, 320}, 0.0f, 0.1f, WHITE);
    }

    DrawCircleV({860, 150}, radius, BLACK);
    DrawTextEx(font, "Black player", {900, 100}, fontSize, spacing, BLACK);
    DrawTextEx(font, "Score", {900, 150}, fontSize, spacing, BLACK);
    DrawText(":", 985, 152, fontSize, BLACK);
    DrawTextEx(font, std::to_string(_scorePlayer1).c_str(), {1000, 150}, fontSize, spacing, BLACK);

    DrawCircleV({860, 350}, radius, WHITE);
    DrawTextEx(font, "White player", {900, 300}, fontSize, spacing, BLACK);
    DrawTextEx(font, "Score", {900, 350}, fontSize, spacing, BLACK);
    DrawText(":", 985, 352, fontSize, BLACK);
    DrawTextEx(font, std::to_string(_scorePlayer2).c_str(), {1000, 350}, fontSize, spacing, BLACK);
}

void HUD::getScores(int scorePlayer1, int scorePlayer2, int currentPlayer)
{
    _scorePlayer1 = scorePlayer1;
    _scorePlayer2 = scorePlayer2;
    _currentPlayer = currentPlayer;
}