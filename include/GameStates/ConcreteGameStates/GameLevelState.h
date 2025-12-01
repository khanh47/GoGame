#pragma once
#include <string>
#include <memory>
#include <vector>
#include "GameState.h"

// Forward declarations
class GameState;
class GameStateModel;
class Scene;
class SceneManager;
class MenuComponent;

class GameLevelState : public GameState {
private:
    mutable int menuVersion = 0;
    std::string selectedGameLevel = "NONE";
    bool gameLevelSelected = false;
    
public:
    void onEnter(GameStateModel* context) override;
    void onExit(GameStateModel* context) override;
    void update(GameStateModel* context, float deltaTime) override;
    
    std::string getName() const { return "GAME_LEVEL"; };
    std::unique_ptr<GameState> clone() const override;
    std::shared_ptr<MenuComponent> createNavigationMenu(GameStateModel* gameStateModel, SceneManager* sceneManager) override;
    virtual std::unique_ptr<Scene> createScene() const override;
    virtual std::vector<std::shared_ptr<MenuItemView>> createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const override;

    // Menu versioning for dynamic menu updates
    int getMenuVersion() const { return menuVersion; }

    // Game mode selection
    void setGameLevelSelected(bool selected, const std::string& mode);
    bool isgameLevelSelected() const { return gameLevelSelected; }
    std::string getSelectedGameLevel() const { return selectedGameLevel; }
};