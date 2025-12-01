#include "GameState.h"
#include "GameModeState.h"
#include "MainMenuState.h"
#include "MenuComponent.h"
#include "MenuItemView.h"
#include "SceneManager.h"
#include "SettingsState.h"

// GameStateModel Implementation
GameStateModel::GameStateModel() {
	// Initialize with a default state, e.g., MainMenuState
	_currentState = std::make_unique<GameModeState>();
	_currentState->onEnter(this);
}

void GameStateModel::setState(std::unique_ptr<GameState> newState) {
	if (_currentState) {
		_currentState->onExit(this);
	}
	_currentState = std::move(newState);
	if (_currentState) {
		_currentState->onEnter(this);
	}
}

void GameStateModel::setStateByName(const std::string &stateName) {
	auto newState = createState(stateName);
	if (newState) {
		setState(std::move(newState));
	}
}

std::string GameStateModel::getCurrentStateName() const {
	if (_currentState) {
		return _currentState->getName();
	}
	return "NONE";
}

void GameStateModel::update(float deltaTime) {
	if (_currentState) {
		_currentState->update(this, deltaTime);
	}
}

std::unique_ptr<GameState> GameStateModel::createState(const std::string &stateName) {
	if (stateName == "MAIN_MENU") {
		return std::make_unique<MainMenuState>();
	}
	// Add other states as needed
	return nullptr;
}

std::shared_ptr<MenuComponent> GameStateModel::createNavigationMenuForCurrentState(SceneManager *sceneManager) {
	if (_currentState) {
		return _currentState->createNavigationMenu(this, sceneManager);
	}
	return nullptr;
}
