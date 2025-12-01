#pragma once
#include "SavedGameListMenuController.h"
#include "SavedGameMenuController.h"
#include <memory>

class DataManager;
class InGameScene;

class SavedGameList {
public:
	SavedGameList(InGameScene *scene, DataManager *data);
	~SavedGameList() = default;

	void open();
	void close();

	bool isOpen() const { return _open; }

	void update();
	void render() const;
	void handleInput();

private:
	void initializeSavedGameListMenuController();
	void initializeSavedGameMenuController();

	bool _open = false;
	bool _shouldUpdate = false;
	std::unique_ptr<SavedGameListMenuController> _listMenuController;
	std::unique_ptr<SavedGameMenuController> _menuController;
	InGameScene *_inGameScene = nullptr;
	DataManager *_gameData = nullptr;
	int _lastMenuVersion = -1;
};
