#pragma once
#include <memory>
#include "DataManager.h"
#include "SavedGameList.h"
#include "TextBox.h"
#include "Game.h"
#include "HUD.h"
#include "Board.h"

// Forward declaration
class InGameScene;
class AudioManager;

class GameController {
private:
	std::string _gameMode;
	InGameScene* _inGameScene = nullptr;
	DataManager* _dataManager = nullptr;
	Game* _game = nullptr;
	Board* _board = nullptr;
	std::unique_ptr<HUD> _hud;
	std::unique_ptr<SavedGameList> _savedGameList;
	std::unique_ptr<TextBox> _textBox;

public:
  	GameController(InGameScene* inGameScene, const std::string& gameMode);
	void init();
	void render();
	bool handleInput();
	void update(float deltaTime);

	bool isGameOver();
	int getScorePlayer1();
	int getScorePlayer2();

	void resetGame();
	void passGame();
	bool undo();
	bool redo();

	void openSaveGameMenu();
	void closeSaveGameMenu();
	void openTextBox();
	void closeTextBox();
	void closeTextBoxAndSave();
	bool isSavingGame() { return _savedGameList->isOpen(); }
};

