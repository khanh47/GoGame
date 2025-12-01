#pragma once
#include "Board.h"
#include "DataManager.h"
#include "Game.h"
#include "HUD.h"
#include "SavedGameList.h"
#include "TextBox.h"
#include <memory>

// Forward declaration
class InGameScene;
class AudioManager;

class GameController {
private:
	std::string _gameMode;
	InGameScene *_inGameScene = nullptr;
    std::unique_ptr<Board> _board;
    std::unique_ptr<Game> _game;
    std::unique_ptr<DataManager> _dataManager;
	std::unique_ptr<HUD> _hud;
	std::unique_ptr<SavedGameList> _savedGameList;
	std::unique_ptr<TextBox> _textBox;
	
    // AI timing
    float _aiThinkTimer = 0.0f;
    float _aiThinkDelay = 0.5f;  // Half second delay before AI moves
    bool _aiIsThinking = false;

public:
	GameController(InGameScene *inGameScene, const std::string &gameMode);
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
