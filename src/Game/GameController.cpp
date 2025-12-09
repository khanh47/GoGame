#include "GameController.h"
#include "InGameScene.h"
#include "SavedGameList.h"
#include "SettingsData.h"
#include "ResourceManager.h"

#include <iostream>
#include <memory>
#include <chrono>

GameController::GameController(InGameScene *inGameScene, const std::string &gameMode)
    : _inGameScene(inGameScene), _gameMode(gameMode) {
    init();
}

GameController::~GameController() {
    if (_aiFuture.valid()) {
        _aiFuture.wait();
    }
}

void GameController::init() {
	int boardSize = SettingsData::getInstance().getBoardSize();
    _game = std::make_unique<Game>(boardSize, boardSize);
    _dataManager = std::make_unique<DataManager>(_game.get());
    _hud = std::make_unique<HUD>(_game.get());
    if (_gameMode != "PVP" && _gameMode != "EASY"
     && _gameMode != "MEDIUM" && _gameMode != "HARD") {
        auto savedFiles = _dataManager->getSavedGamesList();
        std::cout << "Load game from file: " << _gameMode << std::endl;

        for (auto &file : savedFiles) {
            if (file == _gameMode) {
                if (!_dataManager->loadFromFile(file)) {
                    std::cout << "Can't load game's data from file\n";
                }
                break;
            }
        }
    }
    _game->init();
    int dep = _game->getDepth();
    if (dep) {
        _game->enableAI(true, dep, dep > 2);
    }
    _game->replayToIndex(_game->getMoveIndex());
    if (_gameMode == "PVP") {
    } else if (_gameMode == "EASY") {
		_game->enableAI(true, 2, false);
    } else if (_gameMode == "MEDIUM") {
		_game->enableAI(true, 4, true);
    } else if (_gameMode == "HARD") {
		_game->enableAI(true, 6, true);
    }
    _hud->update(_dataManager->getTime());
    _textBox = std::make_unique<TextBox>(_inGameScene, _dataManager.get());
    _savedGameList = std::make_unique<SavedGameList>(_inGameScene, _dataManager.get());
}

void GameController::render() {
    if (!_game || !_hud || !_textBox || ! _savedGameList || !_dataManager)
        return;

    _game->render();
    _hud->render();
    if (_isAIPass) {
        Font font = ResourceManager::getInstance().getFont("Boldonse");
	    DrawTextEx(font, "Bot Passed!", {900, 420}, 40.0f, 1.0f, RED);
    }

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

    if (_aiIsCalculating) {
        return false;
    }
    if (_game->isAITurn()) {
        return false;
    }
    
    if (_game->handleInput()) {
        _isAIPass = false;
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

    _dataManager->update(deltaTime);
    _hud->update(deltaTime);

    if (_game->isAITurn() && !_game->isGameOver()) {
        if (!_aiIsCalculating) {
            startAICalculation();
        } else {
            checkAIResult();
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
		if (_aiFuture.valid()) {
        _aiFuture.wait();
    }
    _aiIsCalculating = false;
    if (_game)
        _game->reset();
}

void GameController::passGame() {
	if (_game) {
		_game->passTurn();
        _isAIPass = false;
    }
}

bool GameController::undo() {
    if (!_game) return false;
    return _game->undo();
}

bool GameController::redo() {
    if (!_game) return false;
    return _game->redo();
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

void GameController::startAICalculation() {
    if (_aiIsCalculating || ! _game || !_game->isAIEnabled()) {
        return;
    }

    _aiIsCalculating = true;
    
    _aiStartTime = std::chrono::high_resolution_clock::now();

    _aiFuture = std::async(std::launch::async, [this]() {
        return _game->calculateAIMove();
    });
}

void GameController::checkAIResult() {
    if (! _aiIsCalculating || !_aiFuture.valid()) {
        return;
    }

    auto status = _aiFuture.wait_for(std::chrono::milliseconds(0));

    if (status == std::future_status::ready) {
        auto aiEndTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(aiEndTime - _aiStartTime).count();
        std::cout << "[AI] calculation time: " << duration << " ms\n";

        auto [row, col] = _aiFuture.get();
        if (row == -1 && col == -1) _isAIPass = true;
        else _isAIPass = false;
        _aiIsCalculating = false;

		if (_game->applyAIMove(row, col)) {
            _shouldPlaySound = true;
        }   
    }}

bool GameController::shouldPlaySound() {
    if (_shouldPlaySound) {
        _shouldPlaySound = false;
        return true;
    }
    return false;
}