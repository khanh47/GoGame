#pragma once
#include "MenuView.h"
#include "MenuItemView.h"

// Concrete implementation of IMenuView for button-style menus
class ButtonMenuView : public IMenuView {
public:
    void createNavigationItemViews(std::shared_ptr<MenuComponent> menuModel, GameState* gameState) override;
    void createItemViews(int numberOfItems, GameState* gameState) override;
    void render(std::shared_ptr<MenuComponent> menuModel) const override;
};