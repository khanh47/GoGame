#pragma once
#include "MenuController.h"
#include <string>

// Forward Declaration
class InGameScene;
class GameModel;

class SavedGameListMenuController : public MenuController {
public:
    SavedGameListMenuController(InGameScene* inGameScene, GameModel* gameModel);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

		void selectGameData(const std::string& mode);
    void createSavedGameListMenu();     // dynamic list

private:
    InGameScene* _inGameScene;
		GameModel* _gameModel;
		int _selectedGameDataIndex = -1;
    std::string _selectedGameData = "";
};
