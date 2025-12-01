#pragma once
#include <string>

class Game;

class HUD {
private:
	Game *_game;
	float _elapsedTime = 0.0f; // total seconds of this match
	bool _timerActive = false;
	std::string formatTime(float seconds);

public:
	HUD(Game *game);
	void update(float deltaTime);
	float getTime() { return _elapsedTime; }
	void render();
};
