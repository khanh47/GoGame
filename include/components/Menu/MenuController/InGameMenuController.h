#pragma once
#include "MenuController.h"
#include <string>

// Forward declaration
class InGameScene;
class IMenuView;

class InGameMenuController : public MenuController {
private:
    InGameScene* _inGameScene;
    int _selectedGameModeIndex = -1;

public:
    InGameMenuController(InGameScene* inGameScene);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createGameModeMenu();
    void selectGameMode(const std::string& mode);

private:
    std::string _selectedGameMode = "NONE";
};
