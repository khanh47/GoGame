#pragma once
#include "MenuController.h"
#include <string>

// Forward Declaration
class InGameScene;
class GameModel;

class SavedGameMenuController : public MenuController {
public:
    SavedGameMenuController(InGameScene* inGameScene, GameModel* gameModel);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createSavedGameMenu();     // dynamic list

private:
    InGameScene* _inGameScene;
    GameModel* _gameModel;
};
