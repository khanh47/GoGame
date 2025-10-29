#include "SceneManager.h"
#include "Scene.h"
#include "InGameScene.h"
#include "MainMenuScene.h"
#include "GameState.h"

#include "raylib.h"
#include <iostream>
#include <cassert>

SceneManager::SceneManager(GameStateModel* gameStateModel)
    : _gameStateModel(gameStateModel) {
    // Constructor with dependency injection (GameState is required)
    if (!_gameStateModel){
        std::cerr << "Error: GameStateModel is required for SceneManager" << std::endl;
        return;
    }
    pushScene(std::make_unique<MainMenuScene>()); // default scene
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

void SceneManager::update(float deltaTime){
    if (_pendingTransition) {
        processTransitions();
    }
    else if (!isEmpty()) {
        auto &currentScene = _sceneStack.top();
        currentScene->handleInput();
        currentScene->update(deltaTime);
    }
}

void SceneManager::render(){
    if (!isEmpty()) {
        auto &currentScene = _sceneStack.top();
        currentScene->render();
    } else {
        std::cerr << "Warning: No scene to render." << std::endl;
    }
}

Scene *SceneManager::getCurrentScene() const {
    if (!isEmpty()) {
        return _sceneStack.top().get();
    }
    return nullptr;
}

std::unique_ptr<Scene> SceneManager::createScene(const std::string &name) {
    return nullptr;
}

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

bool SceneManager::isEmpty() const {
    return _sceneStack.empty();
}

GameStateModel *SceneManager::getGameStateModel() const
{
    return _gameStateModel;
}
