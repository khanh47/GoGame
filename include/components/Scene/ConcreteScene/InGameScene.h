#pragma once
#include "Scene.h"
#include <memory>

// Forward declaration
class GameModel;

class InGameScene : public Scene {
public:
    InGameScene(const std::string &gameMode);
    ~InGameScene() override = default;

    void init(void) override;
    void update(float deltaTime) override;
    void render(void) override;
    void handleInput(void) override;
    void cleanup(void) override;

    bool isActive(void) const override;
    std::string getName(void) const override;
    std::string getGameStateName(void) const override;

    void onEnter(void) override;
    void onExit(void) override;

    bool shouldTransition(void) const override;

private:
    std::shared_ptr<GameModel> _gameModel;
    std::string _gameModeSelected;
};
