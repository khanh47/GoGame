#pragma once
#include <string>
#include "ResourceManager.h"

class HUD {
public:
    HUD();
    void setResourceManager(ResourceManager* resourceManager);
    void render(int _scorePlayer1, int _scorePlayer2, int _currentPlayer);
    void renderPlayersInfo(int _scorePlayer1, int _scorePlayer2, int _currentPlayer);
    void renderTimer();

private:
    ResourceManager* resourceManager;
};