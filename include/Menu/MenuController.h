#pragma once
#include <memory>
#include "MenuView.h"

// Forward declaration
class MenuComponent;
class GameState;
class GameStateModel;

class MenuController {
protected:
    std::shared_ptr<MenuComponent> _menuSystem; // root menu component
    std::shared_ptr<MenuComponent> _currentMenuModel;
    std::unique_ptr<IMenuView> _menuView;

public:
    virtual ~MenuController() = default;

    // Set the view strategy for the menu and create views immediately
    virtual void setViewStrategy(std::unique_ptr<IMenuView> view) = 0;
    virtual void setCurrentMenu(std::shared_ptr<MenuComponent> menu) { _currentMenuModel = menu; }

    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void render() const = 0;
};