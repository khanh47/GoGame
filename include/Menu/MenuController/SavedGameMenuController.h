#pragma once
#include "MenuController.h"

// Forward Declaration
class InGameScene;
class DataManager;

class SavedGameMenuController : public MenuController {
public:
    SavedGameMenuController(InGameScene* inGameScene, DataManager* dataManager);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createSavedGameMenu();     // dynamic list

private:
    InGameScene* _inGameScene;
    DataManager* _dataManager;
};
