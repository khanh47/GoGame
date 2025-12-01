#pragma once
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include "GameState.h"

// Forward declarations
class GameStateModel;
class GameState;
class SceneManager;
class MenuComponent;
class Menu;
class Scene;

class SettingsState : public GameState {
public:
    void onEnter(GameStateModel* context) override;
    void onExit(GameStateModel* context) override;
    void update(GameStateModel* context, float deltaTime) override;
    
    std::string getName() const override { return "SETTINGS"; }
    std::unique_ptr<GameState> clone() const override;
    std::shared_ptr<MenuComponent> createNavigationMenu(GameStateModel* gameStateModel, SceneManager* sceneManager) override;
    virtual std::unique_ptr<Scene> createScene() const override;
    virtual std::vector<std::shared_ptr<MenuItemView>> createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const override;
};

