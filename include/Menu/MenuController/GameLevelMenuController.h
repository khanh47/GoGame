#pragma once
#include "MenuController.h"
#include <string>

// Forward declaration
class GameLevelScene;
class IMenuView;

class GameLevelMenuController : public MenuController {
private:
    GameLevelScene* _GameLevelScene;
    int _selectedGameLevelIndex = -1;

public:
    GameLevelMenuController(GameLevelScene* GameLevelScene);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createGameLevelMenu();
    void selectGameLevel(const std::string& mode);

private:
    std::string _selectedGameLevel = "NONE";
};
