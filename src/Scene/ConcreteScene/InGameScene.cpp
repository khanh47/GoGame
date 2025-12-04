#include "InGameScene.h"
#include "AudioManager.h"
#include "ButtonMenuView.h"
#include "GameController.h"
#include "ResourceManager.h"
#include "SavedGameCommand.h"
#include "raylib.h"
#include <iostream>

InGameScene::InGameScene(const std::string &gameMode) : _gameModeSelected(gameMode) {
	if (_gameModeSelected.empty()) {
		std::cout << "Game mode wasn't selected\n";
		return;
	}
	init();
}

void InGameScene::setDependencies(AudioManager *audioManager) { _audioManager = audioManager; }

void InGameScene::init(void) {
	_gameController = std::make_unique<GameController>(this, _gameModeSelected);
	initializePassButton();
	initializeMenuController();
	_endGameBox = std::make_unique<EndGameBox>();
	std::cout << "InGameScene initialize" << std::endl;
}

void InGameScene::update(float deltaTime) {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return;
	}
	if (_gameController->isGameOver()) {
		if (!_playedWinningSound) {
			std::cout << "[InGameScene] Game over detected, playing winning sound\n";
			if (_audioManager) {
				_audioManager->playSoundEffect("winning_sound");
				_audioManager->stopMusic();
			}
			_playedWinningSound = true;
		}
		_endGameBox->open(_gameController->getScorePlayer1(), _gameController->getScorePlayer2());
	}
	if (_endGameBox->isOpen()) {
		_endGameBox->update();
		return;
	}
	if (_gameController) {
		_gameController->update(deltaTime);
	}
	if (_gameController->isSavingGame()) {
		return;
	}
	if (menuController) {
		menuController->update();
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
	DrawTextureEx(background, {0, 0}, 0.0f, 1.3f, WHITE);
	if (menuController) {
		menuController->render();
	}
	if (_gameController && !_gameController->isSavingGame()) {
		_gameController->render();
	}
	if (_passButton) {
		_passButton->render();
	}
	if (_gameController && _gameController->isSavingGame()) {
		_gameController->render();
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
	if (_gameController->handleInput()) {
		_audioManager->playSoundEffect("placing_stones");
	}
	if (_gameController->isSavingGame())
		return;
	if (_gameController->isAIThinking())
		return;
	if (_passButton)
		_passButton->handleInput();
	if (menuController)
		menuController->handleInput();
}

void InGameScene::cleanup(void) {}

bool InGameScene::isActive(void) const { return _isActive; }

std::string InGameScene::getName(void) const { return "InGameScene"; }

std::string InGameScene::getGameStateName(void) const { return "IN_GAME"; }

void InGameScene::onEnter(void) { _isActive = true; }

void InGameScene::onExit(void) { _isActive = false; }

bool InGameScene::shouldTransition(void) const { return false; }

void InGameScene::initializeMenuController() {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return;
	}
	menuController = std::make_unique<InGameMenuController>(this, _gameController.get());

	menuController->setViewStrategy(std::make_unique<ButtonMenuView>());
	menuController->createInGameMenu();
}

void InGameScene::initializePassButton() {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return;
	}
	_passButton = std::make_unique<PassButton>(_gameController.get());

	_passButton->setViewStrategy(std::make_unique<ButtonMenuView>());
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

void InGameScene::openGameDataInputPopup() { _gameController->openTextBox(); }

void InGameScene::closeGameDataInputPopup() { _gameController->closeTextBox(); }

void InGameScene::closeGameDataInputPopupAndCreate() { _gameController->closeTextBoxAndSave(); }

bool InGameScene::isPopup() {
	if (!_gameController) {
		std::cout << "No game gameController\n";
		return false;
	}
	return _gameController->isSavingGame();
}

bool InGameScene::isAIThinking() {
	if (_gameController) return _gameController->isAIThinking();
	return false;
}
