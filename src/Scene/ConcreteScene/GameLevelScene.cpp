#include "GameLevelScene.h"
#include "SceneManager.h"
#include "GameState.h"
#include "GameLevelState.h"
#include "GameLevelMenuController.h"
#include "ListMenuView.h"
#include "Board.h"
#include "Game.h"
#include "HUD.h"

#include <iostream>
#include "raylib.h"

void GameLevelScene::setDependencies(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    _gameStateModel = gameStateModel;
    _sceneManager = sceneManager;
}

void GameLevelScene::init(void) {
    initializeMenuController();
    std::cout << "GameLevelScene initialized." << std::endl;
}

void GameLevelScene::handleInput(void) {
    if (menuController) {
        menuController->handleInput();
    }
}

void GameLevelScene::update(float deltaTime) {
    if (menuController) {
        menuController->update();
    }
}

void GameLevelScene::render() {
    if (!menuController) return;

    ClearBackground(GRAY);

    menuController->render();
}

void GameLevelScene::cleanup(void) {}

bool GameLevelScene::isActive() const { return _isActive; }

std::string GameLevelScene::getName(void) const { return "GameLevelScene"; }

std::string GameLevelScene::getGameStateName(void) const { return "GAME_LEVEL";}

void GameLevelScene::onEnter(void) {
    // Actions to perform when entering the in-game scene
    _isActive = true;
    init();
}

void GameLevelScene::onExit(void) {
    // Actions to perform when exiting the in-game scene
    _isActive = false;
}

bool GameLevelScene::shouldTransition(void) const { return false; }

void GameLevelScene::selectGameLevel(const std::string& mode) {
    _selectedGameLevel = mode;
    if (menuController) {
        menuController->selectGameLevel(mode);
    }
    // Call GameLevelState to update menu
    if (_gameStateModel) {
        auto* currentState = _gameStateModel->getCurrentState();
        std::cout << "GameLevelScene: current state name " << currentState->getName() << std::endl;
        if (auto* gameLevelState = dynamic_cast<GameLevelState*>(currentState)) {
            gameLevelState->setGameLevelSelected(true, mode);

            if (_sceneManager) {
                _sceneManager->forceMenuRefresh();
                std::cout << "GameLevelScene: force refresh menu" << std::endl;
            }
        }
    }
}

void GameLevelScene::initializeMenuController() {
    menuController = std::make_unique<GameLevelMenuController>(this);

    // Create the ListMenuView with custom positioning
    Rectangle menuArea = {
        (float)(GetScreenWidth() / 2 - 150),  // Center horizontally
        150,                                   // Start below the title
        300,                                   // Width
        400                                    // Height
    };

    auto listMenuView = std::make_unique<ListMenuView>(menuArea);
    listMenuView->setItemHeight(50.0f);
    listMenuView->setItemSpacing(10.0f);
    listMenuView->setScrollbarWidth(20.0f);

    menuController->setViewStrategy(std::move(listMenuView));
    menuController->createGameLevelMenu();
}
