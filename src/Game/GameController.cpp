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
	_board = new Board(19, 19);
	_game = new Game(19, 19, _board);
	_dataManager = new DataManager(_game, _board);
	_hud = std::make_unique<HUD>(_game);

	if (!_dataManager) return;
	int type = 0;
	if (_gameMode == "PVP") {
		_dataManager->addState();
	} else if (_gameMode == "EASY" || _gameMode == "MEDIUM" || _gameMode == "HARD") {
		_dataManager->addState();
	} else {
		auto savedFiles = _dataManager->getSavedGamesList();
		std::cout << "Load game from file: " << _gameMode << std::endl;

		for (auto &file : savedFiles) {
			if (file == _gameMode) {
				if (!_dataManager->loadFromFile(file)) {
					std::cout << "Cant load game's data from file\n";
				}
				break;
			}
		}
	}

	_hud->update(_dataManager->getTime());
	_textBox = std::make_unique<TextBox>(_inGameScene, _dataManager);
	_savedGameList = std::make_unique<SavedGameList>(_inGameScene, _dataManager);
}

void GameController::render() {
	if (!_game || !_hud || !_textBox || !_savedGameList)
		return;
	if (!_dataManager)
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

	_dataManager->update(deltaTime);
	_hud->update(deltaTime);
}

bool GameController::isGameOver() { return _game->isGameOver(); }

int GameController::getScorePlayer1() { return _game->getScorePlayer1(); }

int GameController::getScorePlayer2() { return _game->getScorePlayer2(); }

void GameController::resetGame() {
	if (!_dataManager)
		return;
	_board->reset();
	_game->resetGame();
	std::cout << "After reset: " << _board->getValue(0, 0) << std::endl;
	_dataManager->addState();
	std::cout << "After add state: " << _board->getValue(0, 0) << std::endl;
}

void GameController::passGame() {
	if (!_dataManager)
		return;
	_game->passTurn();
	_dataManager->addState();
}

bool GameController::undo() {
	if (!_dataManager)
		return false;
	return _dataManager->undo();
}

bool GameController::redo() {
	if (!_dataManager)
		return false;
	return _dataManager->redo();
}

void GameController::openSaveGameMenu() { _savedGameList->open(); }

void GameController::closeSaveGameMenu() { _savedGameList->close(); }

void GameController::openTextBox() { _textBox->open(); }

void GameController::closeTextBox() { _textBox->close(); }

void GameController::closeTextBoxAndSave() { _textBox->closeAndCreate(); }
