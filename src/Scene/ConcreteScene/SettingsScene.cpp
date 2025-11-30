#include "SettingsScene.h"
#include "raylib.h"
#include "ButtonMenuView.h"
#include "AudioManager.h"
#include <iostream>

void SettingsScene::setDependencies(AudioManager* audioManager) {
	_audioManager = audioManager;
}

void SettingsScene::init(void) {
  std::cout << "SettingsScene initialized." << std::endl;
	initializeMenuController();
}

void SettingsScene::initializeMenuController() {
  menuController = std::make_unique<SettingsMenuController>(this);

  menuController->setViewStrategy(std::make_unique<ButtonMenuView>());
  menuController->createSettingsMenu();
}

void SettingsScene::handleInput(void) {
}

void SettingsScene::update(float deltaTime) {
}

void SettingsScene::render(void) {
  ClearBackground(GRAY);

	if (menuController)
		menuController->render();
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
