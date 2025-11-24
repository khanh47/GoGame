#pragma once
#include <string>

class HUD {
private:
    int _scorePlayer1;
    int _scorePlayer2;
    int _currentPlayer;
		float _elapsedTime = 0.0f;   // total seconds of this match
    bool _timerActive = false;
		std::string formatTime(float seconds);

public:
    HUD();
		void update(float deltaTime);
    void getScores(int scorePlayer1, int scorePlayer2, int currentPlayer);
    void render();
};
