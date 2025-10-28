#pragma once 
#include <string>
#include <memory>

// Forward declaration
class GameState;
class GameStateModel;
class Scene;

class GameState {
public:
    virtual ~GameState() = default;

    virtual void onEnter(GameStateModel* context) = 0;
    virtual void onExit(GameStateModel* context) = 0;
    virtual void update(GameStateModel* context, float deltaTime) = 0;
    virtual std::string getName() const = 0;
    virtual std::unique_ptr<GameState> clone() const = 0;

    virtual std::unique_ptr<Scene> createScene() const = 0;
};

class GameStateModel {
private:
    std::unique_ptr<GameState> _currentState;

public:
    GameStateModel();
    ~GameStateModel() = default;

    // State management
    void setState(std::unique_ptr<GameState> newState);
    void setStateByName(const std::string& stateName);
    GameState* getCurrentState() const;
    std::string getCurrentStateName() const;

    // Update method
    void update(float deltaTime);

    // Helper method for state pattern
    static std::unique_ptr<GameState> createState(const std::string& stateName);
};