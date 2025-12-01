#include "MainMenuScene.h"
#include "raylib.h"
#include "ResourceManager.h"
#include <iostream>

void MainMenuScene::init(void) {
    std::cout << "MainMenuScene initialized." << std::endl;
}

void MainMenuScene::handleInput(void) {
}

void MainMenuScene::update(float deltaTime) {
}

void MainMenuScene::render(void) {
    Texture2D background = ResourceManager::getInstance().getTexture2D("main_menu_background");
    DrawTextureEx(background, { 0, 0 }, 0.0f, 1.8f, WHITE);
    Font _font = ResourceManager::getInstance().getFont("NinjaKageDemo");
    const int ScreenWidth = GetScreenWidth();
    DrawTextEx(_font, "GO GAME", {450, 50}, 50, 20, BLACK);
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
