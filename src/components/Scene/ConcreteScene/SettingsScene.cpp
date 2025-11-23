#include "SettingsScene.h"
#include "raylib.h"
#include "ResourceManager.h"
#include <iostream>

void SettingsScene::init(void) {
    std::cout << "SettingsScene initialized." << std::endl;
}

void SettingsScene::handleInput(void) {
}

void SettingsScene::update(float deltaTime) {
}

void SettingsScene::render(void) {
    ClearBackground(GRAY);
    Font _font = ResourceManager::getInstance().getFont("NinjaKageDemo");
    DrawTextEx(_font, "SETTINGS", {450, 50}, 50, 20, BLACK);
}

void SettingsScene::cleanup(void) {
    std::cout << "SettingsScene cleaned up." << std::endl;
}

bool SettingsScene::isActive(void) const {
    return _isActive;
}

std::string SettingsScene::getName(void) const {
    return "SettingsScene";
}

std::string SettingsScene::getGameStateName(void) const {
    return "SETTINGS";
}

void SettingsScene::onEnter(void) {
    _isActive = true;
    std::cout << "Entering SettingsScene." << std::endl;
}

void SettingsScene::onExit(void) {
    _isActive = false;
    std::cout << "Exiting SettingsScene." << std::endl;
}

bool SettingsScene::shouldTransition(void) const {
    return false;
}