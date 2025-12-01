#pragma once
#include "MenuController.h"
#include <string>

// Forward declaration
class GameModeScene;
class IMenuView;

class GameModeMenuController : public MenuController {
private:
	GameModeScene *_gameModeScene;
	int _selectedGameModeIndex = -1;

public:
	GameModeMenuController(GameModeScene *gameModeScene);

	void setViewStrategy(std::unique_ptr<IMenuView> view) override;
	void handleInput() override;
	void update() override;
	void render() const override;

	void createGameModeMenu();
	void selectGameMode(const std::string &mode);

private:
	std::string _selectedGameMode = "NONE";
};
