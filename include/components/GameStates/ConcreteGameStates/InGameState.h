#pragma once
#include <string>
#include <memory>

// Forward declarations
class GameState;
class GameStateModel;
class Scene;
class SceneManager;

class InGameState : public GameState {
public:
    void onEnter(GameStateModel* context) override;
    void onExit(GameStateModel* context) override;
    void update(GameStateModel* context, float deltaTime) override;
    std::string getName() const override;
    std::unique_ptr<GameState> clone() const override;
    virtual std::unique_ptr<Scene> createScene() const override;
};