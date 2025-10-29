#include "MainMenuScene.h"
#include "raylib.h"
#include <iostream>

void MainMenuScene::init(void) {
    std::cout << "MainMenuScene initialized." << std::endl;
}

void MainMenuScene::handleInput(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        std::cout << "Enter key pressed in MainMenuScene." << std::endl;
    }
}

void MainMenuScene::update(float deltaTime) {
}

void MainMenuScene::render(void) {
    ClearBackground(RAYWHITE);
    DrawText("Main Menu Scene - Press ENTER to Start", 200, 200, 20, DARKGRAY);
}

void MainMenuScene::cleanup(void) {
    std::cout << "MainMenuScene cleaned up." << std::endl;
}

bool MainMenuScene::isActive(void) const {
    return _isActive;
}

std::string MainMenuScene::getName(void) const {
    return "MainMenuScene";
}

std::string MainMenuScene::getGameStateName(void) const {
    return "MAIN_MENU";
}

void MainMenuScene::onEnter(void) {
    _isActive = true;
    std::cout << "Entering MainMenuScene." << std::endl;
}

void MainMenuScene::onExit(void) {
    _isActive = false;
    std::cout << "Exiting MainMenuScene." << std::endl;
}

bool MainMenuScene::shouldTransition(void) const {
    return false;
}