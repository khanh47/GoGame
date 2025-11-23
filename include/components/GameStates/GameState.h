#pragma once
#include <string>
#include <memory>
#include <vector>

// Forward declaration
class GameState;
class GameStateModel;
class Scene;
class SceneManager;
class MenuComponent;
class MenuItemView;

class GameState {
public:
    virtual ~GameState() = default;

    virtual void onEnter(GameStateModel* context) = 0;
    virtual void onExit(GameStateModel* context) = 0;
    virtual void update(GameStateModel* context, float deltaTime) = 0;
    virtual std::string getName() const = 0;
    virtual std::unique_ptr<GameState> clone() const = 0;

    virtual std::unique_ptr<Scene> createScene() const = 0;
    virtual std::shared_ptr<MenuComponent> createNavigationMenu(GameStateModel* gameStateModel, SceneManager* sceneManager) = 0;
    virtual std::vector<std::shared_ptr<MenuItemView>> createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const = 0;
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
    GameState* getCurrentState() const { return _currentState.get(); };
    std::string getCurrentStateName() const;

    // Update method
    void update(float deltaTime);

    // Helper method for state pattern
    static std::unique_ptr<GameState> createState(const std::string& stateName);

    /// @brief create menu to transit between states
    /// @param sceneManager SceneManager to manage scenes
    /// @return Shared pointer to the created menu component
    std::shared_ptr<MenuComponent> createNavigationMenuForCurrentState(SceneManager* sceneManager);
};
