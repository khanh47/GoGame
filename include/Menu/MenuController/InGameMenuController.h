#pragma once
#include "MenuController.h"

// Forward Declaration
class InGameScene;
class GameController;

class InGameMenuController : public MenuController {
public:
    InGameMenuController(InGameScene* inGameScene, GameController* gameModel);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createInGameMenu();     // dynamic list

private:
    InGameScene* _inGameScene;
    GameController* _gameController;
};

