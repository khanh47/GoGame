#include "InGameScene.h"
#include "SceneManager.h"
#include "GameState.h"
#include "ConcreteGameStates/InGameState.h"
#include "Board.h"
#include "Game.h"
#include "HUD.h"

#include <iostream>
#include "raylib.h"

InGameScene::InGameScene()
{
    _game = new Game(19, 19);
}

void InGameScene::setDependencies(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    _gameStateModel = gameStateModel;
    _sceneManager = sceneManager;
}

void InGameScene::init(void) {
    std::cout << "InGameScene initialized." << std::endl;
}

void InGameScene::handleInput(void) {
    // Handle user input specific to the in-game scene
}

void InGameScene::update(float deltaTime) {
    // Update the in-game scene
}

void InGameScene::render() {
    ClearBackground(GRAY);
    _game->render();
    _game->HandleInput();
    HUD hud;
    hud.setResourceManager(&ResourceManager::getInstance());
    hud.render(_game->scorePlayer1, _game->scorePlayer2, _game->CurrentPlayer);
}

void InGameScene::cleanup(void) {}

bool InGameScene::isActive() const { return _isActive; }

std::string InGameScene::getName(void) const { return "InGameScene"; }

std::string InGameScene::getGameStateName(void) const { return "IN_GAME";}

void InGameScene::onEnter(void) {
    // Actions to perform when entering the in-game scene
    _isActive = true;
    init();
}

void InGameScene::onExit(void) {
    // Actions to perform when exiting the in-game scene
    _isActive = false;
}

bool InGameScene::shouldTransition(void) const { return false; }


