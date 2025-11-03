#pragma once
#include <memory>

// Forward declaration
class HUD;
class Game;

class GameModel {
private:
    std::shared_ptr<HUD> _hud;
    std::shared_ptr<Game> _game;

public:
    GameModel();
    void render();
    void handleInput();
    void update();
};