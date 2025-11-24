#pragma once
#include "Scene.h"
#include <memory>
#include "GameModeMenuController.h"

class GameState;
class GameStateModel;
class SceneManager;
class Game;

class GameModeScene: public Scene {
private:
    std::unique_ptr<GameModeMenuController> menuController;
    std::string _selectedGameMode;
    SceneManager* _sceneManager = nullptr;
    GameStateModel* _gameStateModel = nullptr;

public:
    GameModeScene() = default;
    ~GameModeScene() override = default;

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

    void selectGameMode(const std::string& mode);

private:
    void initializeMenuController();
};
