#pragma once
#include "MenuController.h"
#include <string>

// Forward Declaration
class GameDataScene;
class GameModel;

class GameDataMenuController : public MenuController {
public:
    GameDataMenuController(GameDataScene* gameDataScene);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

		void selectGameData(const std::string& mode);
    void createGameDataMenu();     // dynamic list

private:
    GameDataScene* _gameDataScene;
		int _selectedGameDataIndex = -1;
    std::string _selectedGameData = "";
};

