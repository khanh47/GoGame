#pragma once
#include <memory>
#include <string>
#include <vector>
#include "SavedGameListMenuController.h"
#include "SavedGameMenuController.h"
#include "MenuView.h"

class GameModel;
class InGameScene;

class SavedGameList {
public:
		SavedGameList(InGameScene* scene, GameModel* model);
		~SavedGameList() = default;

		void open();
		void close();

		bool isOpen() const { return _open; }

		void update(float dt);
		void render() const;
		void handleInput();

private:
		void initializeSavedGameListMenuController();
		void initializeSavedGameMenuController();

		bool _open = false;
		std::unique_ptr<SavedGameListMenuController> _listMenuController;
		std::unique_ptr<SavedGameMenuController> _menuController;
		InGameScene* _inGameScene = nullptr;
		GameModel* _gameModel = nullptr;
};
