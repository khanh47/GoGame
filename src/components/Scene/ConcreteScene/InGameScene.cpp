#include "InGameScene.h"
#include "Game.h"
#include "raylib.h"

InGameScene::InGameScene(const std::string &gameMode)
    : _gameModeSelected(gameMode) {
    init();
}

void InGameScene::init(void) {
    // Initialize the game instance with the selected game mode
    _game = std::make_shared<Game>(19, 19);
}

void InGameScene::update(float deltaTime) {
    // Update game logic here if needed
}

void InGameScene::render(void) {
    ClearBackground(RAYWHITE);
    if (_game) {
        _game->render();
    }
}

void InGameScene::handleInput(void) {
    if (_game) {
        _game->HandleInput();
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