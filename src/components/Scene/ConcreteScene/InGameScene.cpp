#include "InGameScene.h"
#include "GameModel.h"
#include "ResourceManager.h"
#include "raylib.h"

InGameScene::InGameScene(const std::string &gameMode)
    : _gameModeSelected(gameMode) {
    init();
}

void InGameScene::init(void) {
    // Initialize the game instance with the selected game mode
    _gameModel = std::make_shared<GameModel>();
}

void InGameScene::update(float deltaTime) {
    // Update game logic here if needed
}

void InGameScene::render(void) {
    Texture2D background = ResourceManager::getInstance().getTexture2D("in_game_background");
    DrawTextureEx(background, { 0, 0 }, 0.0f, 1.3f, WHITE);
    if (_gameModel) {
        _gameModel->render();
    }
}

void InGameScene::handleInput(void) {
    if (_gameModel) {
        _gameModel->handleInput();
    }
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