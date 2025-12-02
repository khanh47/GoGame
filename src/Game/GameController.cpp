#include "GameController.h"
#include "InGameScene.h"
#include "SavedGameList.h"

#include <iostream>
#include <memory>
#include <chrono>

GameController::GameController(InGameScene *inGameScene, const std::string &gameMode)
    : _inGameScene(inGameScene), _gameMode(gameMode) {
    init();
}

GameController::~GameController() {
    // IMPORTANT: Wait for AI thread to finish before destroying
    if (_aiFuture.valid()) {
        _aiFuture. wait();
    }
}

void GameController::init() {
    _board = std::make_unique<Board>(19, 19);
    _game = std::make_unique<Game>(19, 19, _board.get());
    _dataManager = std::make_unique<DataManager>(_game.get(), _board.get());
    _hud = std::make_unique<HUD>(_game.get());

    if (!_dataManager) return;

    if (_gameMode == "PVP") {
        _dataManager->addState();
    } else if (_gameMode == "EASY") {
        _dataManager->addState();
        _game->enableAI(true, 2, 1);  // depth 1
    } else if (_gameMode == "MEDIUM") {
        _dataManager->addState();
        _game->enableAI(true, 2, 2);  // depth 2
    } else if (_gameMode == "HARD") {
        _dataManager->addState();
        _game->enableAI(true, 2, 4);  // depth 3
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
    _textBox = std::make_unique<TextBox>(_inGameScene, _dataManager. get());
    _savedGameList = std::make_unique<SavedGameList>(_inGameScene, _dataManager. get());
}

void GameController::startAICalculation() {
    if (_aiIsCalculating || ! _game || !_game->isAIEnabled()) {
        return;
    }

    _aiIsCalculating = true;

    // Launch AI on separate thread - DOES NOT BLOCK! 
    _aiFuture = std::async(std::launch::async, [this]() {
        return _game->calculateAIMove();
    });
}

void GameController::checkAIResult() {
    if (! _aiIsCalculating || !_aiFuture.valid()) {
        return;
    }

    // Check if done - NON-BLOCKING (0 milliseconds wait)
    auto status = _aiFuture.wait_for(std::chrono::milliseconds(0));
    
    if (status == std::future_status::ready) {
        // AI finished!  Get result
        auto [row, col] = _aiFuture.get();
        _aiIsCalculating = false;

        if (_game->applyAIMove(row, col)) {
            _dataManager->addState();
        }
    }
    // If not ready, just return - game loop continues!
}

void GameController::render() {
    if (!_game || !_hud || !_textBox || ! _savedGameList || !_dataManager)
        return;

    _game->render();
    _hud->render();

    if (_textBox && _textBox->isOpen()) {
        _textBox->render();
    } else if (_savedGameList && _savedGameList->isOpen()) {
        _savedGameList->render();
    }
}

bool GameController::handleInput() {
    if (!_game || !_hud || !_dataManager || !_textBox || !_savedGameList)
        return false;

    if (_textBox->isOpen()) {
        _textBox->handleInput();
        return false;
    }
    if (_savedGameList->isOpen()) {
        _savedGameList->handleInput();
        return false;
    }

    // Don't accept player input while AI is thinking
    if (_aiIsCalculating) {
        return false;
    }

    // Don't accept player input during AI's turn
    if (_game->isAITurn()) {
        return false;
    }

    // Handle player input
    if (_game->handleInput()) {
        _dataManager->addState();
        return true;
    }
    return false;
}

void GameController::update(float deltaTime) {
    if (!_game || !_hud || !_textBox || !_dataManager)
        return;

    if (_textBox->isOpen()) {
        _textBox->update();
        return;
    }
    if (_savedGameList->isOpen()) {
        _savedGameList->update();
        return;
    }

    // ALWAYS update these - this is the key! 
    _dataManager->update(deltaTime);
    _hud->update(deltaTime);

    // Handle AI turn (non-blocking)
    if (_game->isAITurn() && !_game->isGameOver()) {
        if (! _aiIsCalculating) {
            startAICalculation();  // Start background thread
        } else {
            checkAIResult();       // Check if done (instant)
        }
    }
}

bool GameController::isGameOver() { 
    return _game && _game->isGameOver(); 
}

int GameController::getScorePlayer1() { 
    return _game ? _game->getScorePlayer1() : 0; 
}

int GameController::getScorePlayer2() { 
    return _game ? _game->getScorePlayer2() : 0; 
}

void GameController::resetGame() {
    // Wait for AI to finish before resetting
    if (_aiFuture.valid()) {
        _aiFuture.wait();
    }
    _aiIsCalculating = false;

    if (! _dataManager || !_board || !_game)
        return;
    _board->reset();
    _game->resetGame();
    _dataManager->addState();
}

void GameController::passGame() {
    // Wait for AI to finish before passing
    if (_aiFuture.valid()) {
        _aiFuture.wait();
    }
    _aiIsCalculating = false;

    if (!_dataManager || !_game)
        return;
    _game->passTurn();
    _dataManager->addState();
}

bool GameController::undo() {
    // Wait for AI to finish before undo
    if (_aiFuture. valid()) {
        _aiFuture.wait();
    }
    _aiIsCalculating = false;

    if (! _dataManager) return false;
    return _dataManager->undo();
}

bool GameController::redo() {
    // Wait for AI to finish before redo
    if (_aiFuture.valid()) {
        _aiFuture.wait();
    }
    _aiIsCalculating = false;

    if (!_dataManager) return false;
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