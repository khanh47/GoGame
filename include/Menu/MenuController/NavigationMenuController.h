#pragma once
#include "MenuController.h"
#include <memory>

// Forward declaration
class GameState;
class GameStateModel;
class SceneManager;
class IMenuView;
class MenuComponent;

class NavigationMenuController : public MenuController {
private:
	GameStateModel *_gameStateModel;
	SceneManager *_sceneManager;
	std::unique_ptr<GameState> _lastState;

public:
	NavigationMenuController(GameStateModel *gameStateModel, std::shared_ptr<MenuComponent> menuSystem,
													 SceneManager *sceneManager);
	void setViewStrategy(std::unique_ptr<IMenuView> view);
	void updateNavigationMenuForCurrentState();

	void handleInput();
	void update();
	void render() const;

	IMenuView *getMenuView() const;
};