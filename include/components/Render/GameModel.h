#pragma once
#include <memory>

// Forward declaration
class HUD;
class Game;

class GameModel {
private:
    std::shared_ptr<HUD> hud;
    std::shared_ptr<Game> game;

public:
    GameModel();
    void render();
    void handleInput();
    void update();
};