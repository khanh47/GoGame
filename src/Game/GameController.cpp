#include "GameController.h"
#include "InGameScene.h"
#include "SavedGameList.h"

#include <iostream>
#include <memory>

GameController::GameController(InGameScene *inGameScene, const std::string &gameMode)
    : _inGameScene(inGameScene), _gameMode(gameMode) {
    init();
}

void GameController::init() {
    _board = std::make_unique<Board>(19, 19);
    _game = std::make_unique<Game>(19, 19, _board.get());
    _dataManager = std::make_unique<DataManager>(_game.get(), _board. get());
    _hud = std::make_unique<HUD>(_game.get());

    if (! _dataManager) return;

    // Setup based on game mode
    if (_gameMode == "PVP") {
        _dataManager->addState();
    } else if (_gameMode == "EASY") {
        _dataManager->addState();
        _game->enableAI(true, 2, 1);
        _aiThinkDelay = 0.3f;  // Faster for easy
    } else if (_gameMode == "MEDIUM") {
        _dataManager->addState();
        _game->enableAI(true, 2, 2);
        _aiThinkDelay = 0.5f;
    } else if (_gameMode == "HARD") {
        _dataManager->addState();
        _game->enableAI(true, 2, 3);
        _aiThinkDelay = 0.8f;  // Slower for hard (more thinking)
    } else {
        auto savedFiles = _dataManager->getSavedGamesList();
        std::cout << "Load game from file: " << _gameMode << std::endl;

        for (auto &file : savedFiles) {
            if (file == _gameMode) {
                if (! _dataManager->loadFromFile(file)) {
                    std::cout << "Can't load game's data from file\n";
                }
                break;
            }
        }
    }

    _hud->update(_dataManager->getTime());
    _textBox = std::make_unique<TextBox>(_inGameScene, _dataManager.get());
    _savedGameList = std::make_unique<SavedGameList>(_inGameScene, _dataManager.get());
}

void GameController::render() {
    if (! _game || !_hud || !_textBox || ! _savedGameList || !_dataManager)
        return;

    _game->render();
    _hud->render();
    
    // Optional: Show "AI thinking..." indicator
    if (_aiIsThinking) {
        // You can add a visual indicator here
        // DrawText("AI thinking...", 10, 10, 20, GRAY);
    }

    if (_textBox && _textBox->isOpen()) {
        _textBox->render();
    } else if (_savedGameList && _savedGameList->isOpen()) {
        _savedGameList->render();
    }
}

bool GameController::handleInput() {
    if (!_game || !_hud || !_dataManager || !_textBox || ! _savedGameList)
        return false;

    if (_textBox->isOpen()) {
        _textBox->handleInput();
        return false;
    }
    if (_savedGameList->isOpen()) {
        _savedGameList->handleInput();
        return false;
    }

    // Don't accept player input during AI's turn
    if (_game->isAITurn()) {
        return false;
    }

    // Handle player input
    if (_game->handleInput()) {
        _dataManager->addState();
        
        // If AI is enabled and it's now AI's turn, start thinking timer
        if (_game->isAITurn()) {
            _aiIsThinking = true;
            _aiThinkTimer = 0.0f;
        }
        
        return true;
    }
    return false;
}

void GameController::update(float deltaTime) {
    if (!_game || ! _hud || !_textBox || !_dataManager)
        return;

    if (_textBox->isOpen()) {
        _textBox->update();
        return;
    }
    if (_savedGameList->isOpen()) {
        _savedGameList->update();
        return;
    }

    // ALWAYS update these - this was the bug!
    _dataManager->update(deltaTime);
    _hud->update(deltaTime);

    // Handle AI turn with delay
    if (_game->isAITurn() && ! _game->isGameOver()) {
        if (! _aiIsThinking) {
            // Just became AI's turn
            _aiIsThinking = true;
            _aiThinkTimer = 0.0f;
        } else {
            // AI is "thinking"
            _aiThinkTimer += deltaTime;
            
            if (_aiThinkTimer >= _aiThinkDelay) {
                // Time to make the move
                if (_game->makeAIMove()) {
                    _dataManager->addState();
                }
                _aiIsThinking = false;
                _aiThinkTimer = 0.0f;
            }
        }
    }
}

bool GameController::isGameOver() { 
    return _game && _game->isGameOver(); 
}

int GameController::getScorePlayer1() { 
    return _game ?  _game->getScorePlayer1() : 0; 
}

int GameController::getScorePlayer2() { 
    return _game ? _game->getScorePlayer2() : 0; 
}

void GameController::resetGame() {
    if (! _dataManager || !_board || !_game)
        return;
    _board->reset();
    _game->resetGame();
    _dataManager->addState();
    _aiIsThinking = false;
    _aiThinkTimer = 0.0f;
}

void GameController::passGame() {
    if (!_dataManager || ! _game)
        return;
    _game->passTurn();
    _dataManager->addState();
    
    // If it's now AI's turn after pass, start thinking
    if (_game->isAITurn()) {
        _aiIsThinking = true;
        _aiThinkTimer = 0.0f;
    }
}

bool GameController::undo() {
    if (!_dataManager) return false;
    _aiIsThinking = false;
    _aiThinkTimer = 0.0f;
    return _dataManager->undo();
}

bool GameController::redo() {
    if (!_dataManager) return false;
    _aiIsThinking = false;
    _aiThinkTimer = 0.0f;
    return _dataManager->redo();
}

void GameController::openSaveGameMenu() { 
    if (_savedGameList) _savedGameList->open(); 
}

void GameController::closeSaveGameMenu() { 
    if (_savedGameList) _savedGameList->close(); 
}

void GameController::openTextBox() { 
    if (_textBox) _textBox->open(); 
}

void GameController::closeTextBox() { 
    if (_textBox) _textBox->close(); 
}

void GameController::closeTextBoxAndSave() { 
    if (_textBox) _textBox->closeAndCreate(); 
}