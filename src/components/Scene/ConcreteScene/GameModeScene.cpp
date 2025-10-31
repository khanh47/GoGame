#include "GameModeScene.h"
#include "SceneManager.h"
#include "GameState.h"
#include "ConcreteGameStates/GameModeState.h"
#include "GameModeMenuController.h"
#include "ListMenuView.h"
#include "Board.h"
#include "Game.h"
#include "HUD.h"

#include <iostream>
#include "raylib.h"

void GameModeScene::setDependencies(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    _gameStateModel = gameStateModel;
    _sceneManager = sceneManager;
}

void GameModeScene::init(void) {
    _game = new Game(19, 19);
    initializeMenuController();
    std::cout << "GameModeScene initialized." << std::endl;
}

void GameModeScene::handleInput(void) {
    if (menuController) {
        menuController->handleInput();
    }
}

void GameModeScene::update(float deltaTime) {
    if (menuController) {
        menuController->update();
    }
}

void GameModeScene::render() {
    if (!menuController) return;

    ClearBackground(GRAY);

    menuController->render();
}

void GameModeScene::cleanup(void) {}

bool GameModeScene::isActive() const { return _isActive; }

std::string GameModeScene::getName(void) const { return "GameModeScene"; }

std::string GameModeScene::getGameStateName(void) const { return "IN_GAME";}

void GameModeScene::onEnter(void) {
    // Actions to perform when entering the in-game scene
    _isActive = true;
    init();
}

void GameModeScene::onExit(void) {
    // Actions to perform when exiting the in-game scene
    _isActive = false;
}

bool GameModeScene::shouldTransition(void) const { return false; }

void GameModeScene::selectGameMode(const std::string& mode) {
    _selectedGameMode = mode;
    if (menuController) {
        menuController->selectGameMode(mode);
    }
    // Call GameModeState to update menu
    if (_gameStateModel) {
        auto* currentState = _gameStateModel->getCurrentState();
        std::cout << "GameModeScene: current state name " << currentState->getName() << std::endl;
        if (auto* gameModeState = dynamic_cast<GameModeState*>(currentState)) {
            gameModeState->setGameModeSelected(true, mode);
            std::cout << "GameModeScene: bruh " << std::endl;

            if (_sceneManager) {
                _sceneManager->forceMenuRefresh();
                std::cout << "GameModeScene: force refresh menu" << std::endl;
            }
        }
    }
}

void GameModeScene::initializeMenuController() {
    menuController = std::make_unique<GameModeMenuController>(this);

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
    menuController->createGameModeMenu();
}
