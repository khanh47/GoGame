#pragma once
#include <memory>
#include <vector>
#include "MenuItemView.h"

// Forward declaration
class Menu;
class MenuItem;
class GameStateModel;
class GameState;

// Interface for Menu View strategies
class IMenuView {
protected:
    std::vector<std::shared_ptr<MenuItemView>> itemViews;
    
public:
    virtual ~IMenuView() = default;
    virtual void createNavigationItemViews(std::shared_ptr<MenuComponent> menuModel, GameState* gameState) = 0;
    virtual void createItemViews(int numberOfItems, GameState* gameState) = 0;
    virtual void render(std::shared_ptr<MenuComponent> menuModel) const = 0;

    virtual void setItemViews(const std::vector<std::shared_ptr<MenuItemView>>& views) { itemViews = views; }
    std::vector<std::shared_ptr<MenuItemView>> getItemViews() const { return itemViews; }
};