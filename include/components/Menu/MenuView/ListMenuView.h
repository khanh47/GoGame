#pragma once
#include "MenuView.h"
#include "MenuItemView.h"
#include <memory>

// Concrete implementation of IMenuView for list-style menus
class ListMenuView : public IMenuView {
public:
    void createNavigationItemViews(std::shared_ptr<MenuComponent> menuModel, GameState* gameState) override;
    void createItemViews(int numberOfItems, GameState* gameState) override;
    void render(std::shared_ptr<MenuComponent> menuModel) const override;
};