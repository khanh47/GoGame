#include "HUD.h"
#include "Game.h"
#include "ResourceManager.h"
#include "raylib.h"
#include <cmath>

HUD::HUD(Game *game) : _game(game) {
	_elapsedTime = 0.0f; // reset timer
	_timerActive = true;
}

void HUD::update(float deltaTime) { _elapsedTime += deltaTime; }

std::string HUD::formatTime(float seconds) {
	int total = (int)seconds;
	int mins = total / 60;
	int secs = total % 60;

	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%02d:%02d", mins, secs);
	return buffer;
}

void HUD::render() {
	Texture2D &_infoPanel = ResourceManager::getInstance().getTexture2D("info_panel");
	DrawTextureEx(_infoPanel, {750, 35}, 0.0f, 2.5f, WHITE);

	// Draw players info
	Font &font = ResourceManager::getInstance().getFont("Boldonse");
	Texture2D &_ready = ResourceManager::getInstance().getTexture2D("ready_image");

	float fontSize = 40.0f;
	float spacing = 1.0f;
	float radius = 25.0f;

	int currentPlayer = _game->getCurrentPlayer();
	int scorePlayer1 = _game->getScorePlayer1();
	int scorePlayer2 = _game->getScorePlayer2();

	if (currentPlayer == 1) {
		DrawTextureEx(_ready, {1100, 120}, 0.0f, 0.1f, WHITE);
	} else {
		DrawTextureEx(_ready, {1100, 320}, 0.0f, 0.1f, WHITE);
	}

	DrawCircleV({860, 150}, radius, BLACK);
	DrawTextEx(font, "Black player", {900, 100}, fontSize, spacing, BLACK);
	DrawTextEx(font, "Prisoners:", {900, 150}, fontSize, spacing, BLACK);
	DrawTextEx(font, std::to_string(scorePlayer1).c_str(), {1050, 150}, fontSize, spacing, BLACK);

	DrawCircleV({860, 350}, radius, WHITE);
	DrawTextEx(font, "White player", {900, 300}, fontSize, spacing, BLACK);
	DrawTextEx(font, "Prisoners:", {900, 350}, fontSize, spacing, BLACK);
	DrawTextEx(font, std::to_string(scorePlayer2).c_str(), {1050, 350}, fontSize, spacing, BLACK);

	std::string timerStr = formatTime(_elapsedTime);
	DrawTextEx(font, timerStr.c_str(), {940, 600}, 50, 2, BLACK);
}
