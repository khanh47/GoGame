#include "InGameScene.h"
#include "GameController.h"
#include "ResourceManager.h"
#include "ButtonMenuView.h"
#include "SavedGameCommand.h"
#include "raylib.h"
#include <iostream>

InGameScene::InGameScene(const std::string &gameMode)
    : _gameModeSelected(gameMode) {
	if (_gameModeSelected.empty()) {
		std::cout << "Game mode wasn't selected\n";
		return;
	}
	init();
}

void InGameScene::init(void) {
	_gameController = new GameController(this, _gameModeSelected);
	initializePassButton();
	initializeMenuController();
	_textBox = std::make_unique<TextBox>(this);
	_endGameBox = std::make_unique<EndGameBox>();
	std::cout << "InGameScene initialize" << std::endl;
}

void InGameScene::update(float deltaTime) {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return;
	}
	if (_gameController->isGameOver()) {
		_endGameBox->open(_gameController->getScorePlayer1(), _gameController->getScorePlayer2());
	}
	if (_endGameBox->isOpen()) {
		_endGameBox->update();
		return;
	}
	if (_textBox->isOpen()) {
		_textBox->update(deltaTime);
		return;
	}
	if (_gameController->isSavingGame()) {
		return;
	}
  if (menuController) {
    menuController->update();
	}

  if (_gameController) {
    _gameController->update(deltaTime);
	}

	if (_passButton) {
		_passButton->update();
	}
}

void InGameScene::render(void) {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return;
	}
  Texture2D background = ResourceManager::getInstance().getTexture2D("in_game_background");
  DrawTextureEx(background, { 0, 0 }, 0.0f, 1.3f, WHITE);
	if (menuController) {
		menuController->render();
	}
	if (_passButton) {
		_passButton->render();
	}
  if (_gameController) {
    _gameController->render();
  }
	if (_textBox->isOpen()) {
		_textBox->render();
	}
	if (_endGameBox->isOpen()) {
		_endGameBox->render();
	}
}

void InGameScene::handleInput() {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return;
	}
		if (_endGameBox->isOpen()) {
				_endGameBox->handleInput();
				return;
		}
		if (_textBox->isOpen()) {
				_textBox->handleInput();
				return;
		}

		if (_passButton)
				_passButton->handleInput();
    if (menuController)
        menuController->handleInput();
    if (_gameController)
        _gameController->handleInput();
}

void InGameScene::cleanup(void) {
}

bool InGameScene::isActive(void) const {
    return _isActive;
}

std::string InGameScene::getName(void) const {
    return "InGameScene";
}

std::string InGameScene::getGameStateName(void) const {
    return "IN_GAME";
}

void InGameScene::onEnter(void) {
    _isActive = true;
}

void InGameScene::onExit(void) {
    _isActive = false;
}

bool InGameScene::shouldTransition(void) const {
    return false;
}

void InGameScene::initializeMenuController() {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return;
	}
    menuController = std::make_unique<InGameMenuController>(this, _gameController);

    menuController->setViewStrategy(std::make_unique<ButtonMenuView>());
    menuController->createInGameMenu();
}

void InGameScene::initializePassButton() {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return;
	}
    _passButton = std::make_unique<PassButton>(_gameController);

    _passButton ->setViewStrategy(std::make_unique<ButtonMenuView>());
    _passButton->createPassButton();
}

void InGameScene::openSavedGameListPopup() {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return;
	}
	_gameController->openSaveGameMenu();
}

void InGameScene::closeSavedGameListPopup() {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return;
	}
	_gameController->closeSaveGameMenu();
}

void InGameScene::openGameDataInputPopup() {
		_textBox->open();
}

void InGameScene::closeGameDataInputPopup() {
		_textBox->close();
}

void InGameScene::closeGameDataInputPopupAndCreate() {
		_textBox->closeAndCreate();
}

bool InGameScene::isPopup() {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return false;
	}
		return _gameController->isSavingGame() || _textBox->isOpen();
}
