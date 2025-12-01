#pragma once
#include "MenuController.h"
#include <string>

// Forward Declaration
class InGameScene;
class DataManager;

class SavedGameListMenuController : public MenuController {
public:
    SavedGameListMenuController(DataManager* dataManager);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

		void selectGameData(const std::string& mode);
    void createSavedGameListMenu();     // dynamic list

private:
		DataManager* _dataManager;
		int _selectedGameDataIndex = -1;
    std::string _selectedGameData = "";
};
