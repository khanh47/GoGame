#pragma once
#include <memory>
#include <vector>
#include "MenuItemView.h"

// Forward declaration
class GameState;
class MenuComponent;

// Interface for Menu View strategies
class IMenuView {
protected:
    std::vector<std::shared_ptr<MenuItemView>> _itemViews;

public:
    virtual ~IMenuView() = default;
    virtual void createNavigationItemViews(std::shared_ptr<MenuComponent> menuModel, GameState* gameState) = 0;
    virtual void createInGameItemsViews(int numberOfItems) = 0;
    virtual void createSavedGameItemsViews(int numberOfItems) = 0;
    virtual void createSettingMenuItemViews(int numberOfItems) = 0;
    virtual void render(std::shared_ptr<MenuComponent> menuModel) const = 0;

    virtual void setItemViews(const std::vector<std::shared_ptr<MenuItemView>>& views) { _itemViews = views; }
    std::vector<std::shared_ptr<MenuItemView>> getItemViews() const { return _itemViews; }
};
