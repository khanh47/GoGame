#pragma once
#include <memory>
#include "DataManager.h"
#include "SavedGameList.h"
#include "Game.h"
#include "HUD.h"
#include "Board.h"

// Forward declaration
class InGameScene;

class GameController {
private:
	std::string _gameMode;
	InGameScene* _inGameScene = nullptr;
	DataManager* _dataManager = nullptr;
	Game* _game = nullptr;
	Board* _board = nullptr;
	std::unique_ptr<HUD> _hud;
  std::unique_ptr<SavedGameList> _savedGameList;

public:
  GameController(InGameScene* inGameScene, const std::string& gameMode);
	void init();
  void render();
  void handleInput();
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
	bool isSavingGame() { return _savedGameList->isOpen(); }
};

