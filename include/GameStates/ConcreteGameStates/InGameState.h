#pragma once
#include "GameState.h"

// Forward declarations
class GameStateModel;
class GameState;
class MenuComponent;
class SceneManager;
class MenuItemView;
class Scene;

class InGameState : public GameState {
private:
    std::string _gameModeSelected = "NONE";

public:
    InGameState(const std::string& mode) : _gameModeSelected(mode) {};

    void onEnter(GameStateModel* context) override;
    void onExit(GameStateModel* context) override;
    void update(GameStateModel* context, float deltaTime) override;
    std::string getName() const override { return "IN_GAME"; }
    std::unique_ptr<GameState> clone() const override;
    std::unique_ptr<Scene> createScene() const override;
    std::shared_ptr<MenuComponent> createNavigationMenu(GameStateModel* gameStateModel, SceneManager* sceneManager) override;
    std::vector<std::shared_ptr<MenuItemView>> createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const override;
};
