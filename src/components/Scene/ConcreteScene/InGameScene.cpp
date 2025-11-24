#include "InGameScene.h"
#include "GameModel.h"
#include "ResourceManager.h"
#include "ButtonMenuView.h"
#include "SavedGameCommand.h"
#include "ListMenuView.h"
#include "raylib.h"
#include <iostream>

InGameScene::InGameScene(const std::string &gameMode)
    : _gameModeSelected(gameMode) {
		init();
}

void InGameScene::init(void) {
		if (_gameModeSelected.empty()) return;
		_gameModel = new GameModel();
		if (_gameModeSelected != "PVP" && _gameModeSelected != "PVE") {
		    auto savedFiles = _gameModel->getSavedGamesList();
				std::cout << "Game data: " << _gameModeSelected << std::endl;

		    for (auto& file : savedFiles) {
		        if (file == _gameModeSelected) {
		            auto snapOpt = _gameModel->loadFromFile(file);

		            if (snapOpt.has_value()) {
										_gameModel->pushState(snapOpt.value());
		            }

		            break;
		        }
		    }
		}
		initializePassButton();
		initializeMenuController();
		_savedGameList = std::make_unique<SavedGameList>(this, _gameModel);
		_textBox = std::make_unique<TextBox>(this);
		_endGameBox = std::make_unique<EndGameBox>(_gameModel->finalScorePlayer1(), _gameModel->finalScorePlayer2());
		std::cout << "InGameScene initialize" << std::endl;
}

void InGameScene::update(float deltaTime) {
		if (_endGameBox->isOpen()) {
				_endGameBox->update();
				return;
		}
		if (_textBox->isOpen()) {
				_textBox->update(deltaTime);
				return;
		}
    if (_savedGameList->isOpen()) {
				_savedGameList->update(deltaTime);
        return;
    }

    if (menuController)
        menuController->update();

    if (_gameModel)
        _gameModel->update(deltaTime);

		if (_passButton)
				_passButton->update();
		if (_gameModel->isGameOver()) {
				_endGameBox->open();
		}
}

void InGameScene::render(void) {
    Texture2D background = ResourceManager::getInstance().getTexture2D("in_game_background");
    DrawTextureEx(background, { 0, 0 }, 0.0f, 1.3f, WHITE);
    if (_gameModel) {
        _gameModel->render();
    }
		if (menuController) {
				menuController->render();
		}
		if (_passButton) {
				_passButton->render();
		}
		if (_savedGameList->isOpen()) {
				_savedGameList->render();
		}
		if (_textBox->isOpen()) {
				_textBox->render();
		}
		if (_endGameBox->isOpen()) {
				_endGameBox->render();
		}
}

void InGameScene::handleInput() {
		if (_endGameBox->isOpen()) {
				_endGameBox->handleInput();
				return;
		}
		if (_textBox->isOpen()) {
				_textBox->handleInput();
				return;
		}
		if (_savedGameList->isOpen()) {
				_savedGameList->handleInput();
				return;
		}

		if (_passButton)
				_passButton->handleInput();
    if (menuController)
        menuController->handleInput();
    if (_gameModel)
        _gameModel->handleInput();
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
    menuController = std::make_unique<InGameMenuController>(this, _gameModel);

    menuController->setViewStrategy(std::make_unique<ButtonMenuView>());
    menuController->createInGameMenu();
}

void InGameScene::initializePassButton() {
    _passButton = std::make_unique<PassButton>(_gameModel);

    _passButton ->setViewStrategy(std::make_unique<ButtonMenuView>());
    _passButton->createPassButton();
}

void InGameScene::openSavedGameListPopup() {
		_savedGameList->open();
}

void InGameScene::closeSavedGameListPopup() {
		_savedGameList->close();
}

void InGameScene::openGameDataInputPopup() {
		_textBox->open();
}

void InGameScene::closeGameDataInputPopup() {
		_textBox->close();
}

void InGameScene::openEndGameBoxPopup() {
		_endGameBox->open();
}

void InGameScene::closeEndGameBoxPopup() {
		_endGameBox->close();
}

bool InGameScene::isPopup() {
		return _savedGameList->isOpen() || _textBox->isOpen();
}
