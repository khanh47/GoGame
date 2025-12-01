#include "SettingsScene.h"
#include "AudioManager.h"
#include "ButtonMenuView.h"
#include "SceneManager.h"
#include "raylib.h"
#include <iostream>

void SettingsScene::setDependencies(AudioManager *audioManager, SceneManager *sceneManager) {
	_audioManager = audioManager;
	_sceneManager = sceneManager;
}

void SettingsScene::init(void) {
	std::cout << "SettingsScene initialized." << std::endl;
	initializeMenuController();
}

void SettingsScene::initializeMenuController() {
	menuController = std::make_unique<SettingsMenuController>(this, _audioManager);
	menuController->createSettingsMenu();
}

void SettingsScene::handleInput(void) {
	if (menuController) {
		menuController->handleInput();
	}
}

void SettingsScene::update(float deltaTime) {
	if (menuController) {
		menuController->update();
	}

	// Handle pop scene request
	if (_shouldPopScene && _sceneManager) {
		_sceneManager->popScene();
		_shouldPopScene = false;
	}
}

void SettingsScene::render(void) {
	ClearBackground(GRAY);

	if (menuController)
		menuController->render();
}

void SettingsScene::cleanup(void) { std::cout << "SettingsScene cleaned up." << std::endl; }

bool SettingsScene::isActive(void) const { return _isActive; }

std::string SettingsScene::getName(void) const { return "SettingsScene"; }

std::string SettingsScene::getGameStateName(void) const { return "SETTINGS"; }

void SettingsScene::onEnter(void) {
	_isActive = true;
	_shouldPopScene = false;
	std::cout << "Entering SettingsScene." << std::endl;
}

void SettingsScene::onExit(void) {
	_isActive = false;
	std::cout << "Exiting SettingsScene." << std::endl;
}

bool SettingsScene::shouldTransition(void) const { return false; }

void SettingsScene::requestPopScene() { _shouldPopScene = true; }
