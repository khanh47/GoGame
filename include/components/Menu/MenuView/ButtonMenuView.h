#pragma once
#include "MenuView.h"
#include <memory>

// Forward declaration
class MenuComponent;
class GameState;

// Concrete implementation of IMenuView for button-style menus
class ButtonMenuView : public IMenuView {
public:
    ButtonMenuView() = default;

    void createNavigationItemViews(std::shared_ptr<MenuComponent> menuModel, GameState* gameState) override;
    void createSettingMenuItemViews(int numberOfItems) override;
    void createInGameItemsViews(int numberOfItems) override;
    void createSavedGameItemsViews(int numberOfItems) override;
    void createPassButton(int numberOfItems) override;
    void render(std::shared_ptr<MenuComponent> menuModel) const override;
};
