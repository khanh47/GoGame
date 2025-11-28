#include "EndGameBox.h"
#include "ResourceManager.h"
#include "colors.h"
#include "raylib.h"

void EndGameBox::open(const int& scorePlayer1, const int& scorePlayer2) {
    if (_open) return;
    _open = true;
		_scorePlayer1 = scorePlayer1;
		_scorePlayer2 = scorePlayer2;
}

void EndGameBox::close() {
    _open = false;
}

void EndGameBox::update() {
}

void EndGameBox::render() const {
    if (!_open) return;

    int screenWidth  = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));

    Rectangle box = {
        (screenWidth  - 800) / 2.0f,
        (screenHeight - 600) / 2.0f,
        800,
        600
    };

    DrawRectangleRounded(box, 0.2f, 16, Fade({220, 220, 220, 255}, 0.97f));

    Font font = ResourceManager::getInstance().getFont("Boldonse");

    float titleSize = 70.0f;
    float textSize  = 40.0f;
    float spacing   = 2.0f;

    float xCenter = box.x + box.width / 2.0f;

    const char* title = "END GAME";
    Vector2 titleSizeVec = MeasureTextEx(font, title, titleSize, spacing);

    DrawTextEx(
        font,
        title,
        { xCenter - titleSizeVec.x / 2.0f, box.y + 40 },
        titleSize,
        spacing,
        BLACK
    );

    std::string p1str = "Player 1: " + std::to_string(_scorePlayer1);
    std::string p2str = "Player 2: " + std::to_string(_scorePlayer2);

    Vector2 p1Size = MeasureTextEx(font, p1str.c_str(), textSize, spacing);
    Vector2 p2Size = MeasureTextEx(font, p2str.c_str(), textSize, spacing);

    DrawTextEx(
        font,
        p1str.c_str(),
        { xCenter - p1Size.x / 2.0f, box.y + 180 },
        textSize,
        spacing,
        BLACK
    );

    DrawTextEx(
        font,
        p2str.c_str(),
        { xCenter - p2Size.x / 2.0f, box.y + 240 },
        textSize,
        spacing,
        BLACK
    );

    std::string winner;

    if (_scorePlayer1 > _scorePlayer2)
        winner = "Black Player Wins!";
    else if (_scorePlayer2 > _scorePlayer1)
        winner = "White Player Wins!";
    else
        winner = "Draw!";

    Vector2 winSize = MeasureTextEx(font, winner.c_str(), textSize + 10.f, spacing);

    DrawTextEx(
        font,
        winner.c_str(),
        { xCenter - winSize.x / 2.0f, box.y + 330 },
        textSize + 10.f,
        spacing,
        BLACK
    );
}

void EndGameBox::handleInput() {
}
