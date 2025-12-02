#pragma once
#include "Board.h"
#include "DataManager.h"
#include "Game.h"
#include "HUD.h"
#include "SavedGameList.h"
#include "TextBox.h"
#include <memory>
#include <future>
#include <atomic>

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

    // Async AI
    std::future<std::pair<int, int>> _aiFuture;
    std::atomic<bool> _aiIsCalculating{false};

public:
    GameController(InGameScene *inGameScene, const std::string &gameMode);
    ~GameController();
    
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
    bool isSavingGame() { return _savedGameList && _savedGameList->isOpen(); }
    
    // Check if AI is thinking (for UI feedback)
    bool isAIThinking() const { return _aiIsCalculating; }

private:
    void startAICalculation();
    void checkAIResult();
};