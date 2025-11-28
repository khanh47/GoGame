#include "GameDataScene.h"
#include "SceneManager.h"
#include "GameState.h"
#include "ConcreteGameStates/GameDataState.h"
#include "GameDataMenuController.h"
#include "ListMenuView.h"
#include "Board.h"
#include "Game.h"
#include "HUD.h"
#include "GameController.h"

#include <iostream>
#include "raylib.h"

void GameDataScene::setDependencies(GameStateModel* gameStateModel, SceneManager* sceneManager)
{
    _gameStateModel = gameStateModel;
    _sceneManager = sceneManager;
}

void GameDataScene::init(void) {
    initializeMenuController();
    std::cout << "GameDataScene initialized." << std::endl;
}

void GameDataScene::handleInput(void) {
    if (menuController) {
        menuController->handleInput();
    }
}

void GameDataScene::update(float deltaTime) {
    if (menuController) {
        menuController->update();
    }
}

void GameDataScene::render() {
    if (!menuController) return;

    ClearBackground(GRAY);

    menuController->render();
}

void GameDataScene::cleanup(void) {}

bool GameDataScene::isActive() const { return _isActive; }

std::string GameDataScene::getName(void) const { return "GameDataScene"; }

std::string GameDataScene::getGameStateName(void) const { return "GAME_DATA"; }

void GameDataScene::onEnter(void) {
    // Actions to perform when entering the in-game scene
    _isActive = true;
    init();
}

void GameDataScene::onExit(void) {
    // Actions to perform when exiting the in-game scene
    _isActive = false;
}

bool GameDataScene::shouldTransition(void) const { return false; }

void GameDataScene::selectGameData(const std::string& data) {
    _selectedGameData = data;
    if (menuController) {
        menuController->selectGameData(data);
    }
    // Call GameDataState to update menu
    if (_gameStateModel) {
        auto* currentState = _gameStateModel->getCurrentState();
        std::cout << "GameDataScene: current state name " << currentState->getName() << std::endl;
        if (auto* gameDataState = dynamic_cast<GameDataState*>(currentState)) {
            gameDataState->setGameDataSelected(true, data);

            if (_sceneManager) {
                _sceneManager->forceMenuRefresh();
                std::cout << "GameDataScene: force refresh menu" << std::endl;
            }
        }
    }
}

void GameDataScene::initializeMenuController() {
    menuController = std::make_unique<GameDataMenuController>(this);

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
    menuController->createGameDataMenu();
}

