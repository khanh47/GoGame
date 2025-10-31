#include "SceneManager.h"
#include "Scene.h"
#include "InGameScene.h"
#include "MainMenuScene.h"
#include "GameState.h"
#include "NavigationMenuController.h"
#include "MenuComponent.h"
#include "ButtonMenuView.h"

#include "raylib.h"
#include <iostream>
#include <cassert>

// Scene manager implementation
SceneManager::SceneManager(GameStateModel* gameStateModel)
    : _gameStateModel(gameStateModel) {
    // Constructor with dependency injection (GameState is required)
    if (!_gameStateModel){
        std::cerr << "Error: GameStateModel is required for SceneManager" << std::endl;
        return;
    }
    initializeMenuSystem();
    pushScene(std::make_unique<MainMenuScene>()); // default scene
    _menuActive = true;
}

SceneManager::~SceneManager() {
    while (!isEmpty()) {
        popScene();
    }
}

void SceneManager::pushScene(std::unique_ptr<Scene> scene){
    // check if the scene was created succesfully
    if (scene){
        // Inject dependencies if needed
        if (auto* inGameScene = dynamic_cast<InGameScene*>(scene.get())) {
            inGameScene->setDependencies(_gameStateModel, this);
        }
        _sceneStack.emplace(std::move(scene));
        auto &entry = _sceneStack.top();
        entry->init();
        entry->onEnter();
        std::cout << "Pushed scene: " << entry->getName() << std::endl;
    }
}

void SceneManager::popScene(){
    if (!isEmpty()) {
        auto &topScene = _sceneStack.top();
        topScene->onExit();
        topScene->cleanup();
        _sceneStack.pop();
    }
}

void SceneManager::changeScene(std::unique_ptr<Scene> scene){
    _nextScene = std::move(scene);
    _pendingTransition = true;
    _isChangeScene = true;
}

void SceneManager::pushSceneDeferred(std::unique_ptr<Scene> scene) {
    _nextScene = std::move(scene);
    _pendingTransition = true;
    _isChangeScene = false;
}

// Core loop methods
void SceneManager::update(float deltaTime){
    if (_pendingTransition) {
        processTransitions();
    }
    else if (!isEmpty()) {
        auto &currentScene = _sceneStack.top();
        if (currentScene->isActive()) {
            currentScene->handleInput();
            currentScene->update(deltaTime);
        }
    }
    forceMenuRefresh();

    // update menu system if active
    if (_menuActive && _navigationMenuController) {
        updateMenuSystem(deltaTime);
    }

    // Handle toggle menu input
    if (IsKeyPressed(KEY_TAB)) {
        toggleMenu();
    }
}

void SceneManager::render(){
    if (!isEmpty()) {
        auto &currentScene = _sceneStack.top();
        if (currentScene->isActive()) {
            currentScene->render();
    }
    } else {
        std::cerr << "Warning: No scene to render." << std::endl;
    }

    forceMenuRefresh();
    // Render menu system if active
    if (_menuActive && _navigationMenuController) {
        renderMenuSystem();
    }
}

// Scene queries
Scene *SceneManager::getCurrentScene() const {
    if (!isEmpty()) {
        return _sceneStack.top().get();
    }
    return nullptr;
}

bool SceneManager::isEmpty() const {
    return _sceneStack.empty();
}

// Menu management
void SceneManager::showMenu() {
    _menuActive = true;
}

void SceneManager::hideMenu() {
    _menuActive = false;
}

bool SceneManager::isMenuActive() const {
    return _menuActive;
}

void SceneManager::toggleMenu() {
    if (_menuActive) {
        hideMenu();
    } else {
        showMenu();
    }
}

void SceneManager::forceMenuRefresh() {
    if (_navigationMenuController) {
        _navigationMenuController->updateNavigationMenuForCurrentState();
    }
}

// Game state model access
GameStateModel *SceneManager::getGameStateModel() const
{
    return _gameStateModel;
}

// private methods
void SceneManager::processTransitions() {
    assert(_pendingTransition && _nextScene != nullptr);
    
    std::string sceneName = _nextScene->getName();

    if (_isChangeScene) {
        popScene();
        _isChangeScene = false;
    }

    pushScene(std::move(_nextScene));

    _nextScene = nullptr;
    _pendingTransition = false;
}

std::unique_ptr<Scene> SceneManager::createScene(const std::string &name) {
    return nullptr;
}

void SceneManager::initializeMenuSystem() {
    if (!_gameStateModel){
        std::cerr << "Error: GameStateModel is required to initialize menu system" << std::endl;
        return;
    }
    _gameStateModel->setStateByName("MAIN_MENU"); // default state
    // Create menu controller and menu system
    _navigationMenuSystem = _gameStateModel->createNavigationMenuForCurrentState(this);
    _navigationMenuController = std::make_shared<NavigationMenuController>(_gameStateModel, _navigationMenuSystem, this);
    // Set a default view strategy
    _navigationMenuController->setViewStrategy(std::make_unique<ButtonMenuView>());
    std::cout << "Menu system initialized." << std::endl;
}

void SceneManager::updateMenuSystem(float deltaTime) {
    if (_navigationMenuController) {
        _navigationMenuController->update();
    }
}

void SceneManager::renderMenuSystem() {
    if (_navigationMenuController) {
        _navigationMenuController->render();
    }
}