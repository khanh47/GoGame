#pragma once
#include "Scene.h"
#include <string>
#include <memory>

class GameState;
class GameStateModel;
class SceneManager;
class Game;

class InGameScene: public Scene {
private:
    SceneManager* _sceneManager = nullptr;
    GameStateModel* _gameStateModel = nullptr;
    Game* _game = nullptr;

public:
    InGameScene();
    ~InGameScene() override = default;

    // Set dependencies for state communication
    void setDependencies(GameStateModel* gameStateModel, SceneManager* sceneManager);
    
    void init(void) override;
    void handleInput(void) override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;

    bool isActive() const override;
    std::string getName(void) const override;
    std::string getGameStateName(void) const override;

    void onEnter(void) override;
    void onExit(void) override;

    bool shouldTransition(void) const override;
};