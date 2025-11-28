#include "GameController.h"
#include "InGameScene.h"
#include "SavedGameList.h"

#include <iostream>
#include <memory>

GameController::GameController(InGameScene* inGameScene, const std::string& gameMode) : _inGameScene(inGameScene), _gameMode(gameMode)
{
	init();
}

void GameController::init() {
	_board = new Board(19, 19);
	_game = new Game(19, 19, _board);
	_dataManager = new DataManager(_game, _board);
	_hud = std::make_unique<HUD>(_game);

	if (!_dataManager) return;
	if (_gameMode != "PVP" && _gameMode != "PVE") {
		auto savedFiles = _dataManager->getSavedGamesList();
		std::cout << "Game data: " << _gameMode << std::endl;

		for (auto& file : savedFiles) {
			if (file == _gameMode) {
				if (!_dataManager->loadFromFile(file)) {
					std::cout << "Cant load game's data from file\n";
				}
				break;
			}
		}
	} else {
		_dataManager->addState();
	}

	_savedGameList = std::make_unique<SavedGameList>(_inGameScene, _dataManager);
}

void GameController::render() {
  if (!_game || !_hud) return;
	if (!_dataManager) return;
  _game->render();
  _hud->render();
	if (_savedGameList->isOpen()) {
		_savedGameList->render();
	}
}

void GameController::handleInput() {
  if (!_game || !_hud) return;
	if (!_dataManager) return;
	if (_savedGameList->isOpen()) {
		_savedGameList->handleInput();
		return;
	}
  if (_game->handleInput()) {
		_dataManager->addState();
	}
}

void GameController::update(float deltaTime) {
  if (!_game || !_hud || !_dataManager) return;
  if (_savedGameList->isOpen()) {
		_savedGameList->update();
  }

	_dataManager->update();
	_hud->update(deltaTime);
}

bool GameController::isGameOver() {
		return _game->isGameOver();
}

int GameController::getScorePlayer1() {
		return _game->getScorePlayer1();
}

int GameController::getScorePlayer2() {
		return _game->getScorePlayer2();
}

void GameController::resetGame() {
	if (!_dataManager) return;
	_board->reset();
	_game->resetGame();
	std::cout << "After reset: " << _board->getValue(0, 0) << std::endl;
	_dataManager->addState();
	std::cout << "After add state: " << _board->getValue(0, 0) << std::endl;
}

void GameController::passGame() {
	if (!_dataManager) return;
	_game->passTurn();
	_dataManager->addState();
}

bool GameController::undo() {
	if (!_dataManager) return false;
	return _dataManager->undo();
}

bool GameController::redo() {
	if (!_dataManager) return false;
	return _dataManager->redo();
}

void GameController::openSaveGameMenu() {
	_savedGameList->open();
}

void GameController::closeSaveGameMenu() {
	_savedGameList->close();
}
