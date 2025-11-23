#pragma once
#include "MenuController.h"
#include <string>

// Forward Declaration
class InGameScene;
class GameModel;

class InGameMenuController : public MenuController {
public:
    InGameMenuController(InGameScene* inGameScene, GameModel* gameModel);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createInGameMenu();     // dynamic list

private:
    InGameScene* _inGameScene;
    GameModel* _gameModel;
};

