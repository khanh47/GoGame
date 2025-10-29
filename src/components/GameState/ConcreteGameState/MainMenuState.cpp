#include "GameState.h"
#include "ConcreteGameStates/MainMenuState.h"
#include "ConcreteScene/MainMenuScene.h"
#include "Scene.h"
#include <iostream>

void MainMenuState::onEnter(GameStateModel *context) {
    std::cout << "Entering Main Menu State" << std::endl;
}

void MainMenuState::onExit(GameStateModel *context) {
    std::cout << "Exiting Main Menu State" << std::endl;
}

void MainMenuState::update(GameStateModel *context, float deltaTime) {
}

std::string MainMenuState::getName() const {
    return "MAIN_MENU";
}

std::unique_ptr<GameState> MainMenuState::clone() const {
    return std::make_unique<MainMenuState>();
}

std::unique_ptr<Scene> MainMenuState::createScene() const {
    return std::make_unique<MainMenuScene>();
}