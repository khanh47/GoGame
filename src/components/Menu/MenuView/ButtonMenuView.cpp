#include "ButtonMenuView.h"
#include "MenuComponent.h"
#include "MenuItem.h"
#include "GameState.h"
#include "MenuView.h"
#include "ResourceManager.h"

void ButtonMenuView::createNavigationItemViews(std::shared_ptr<MenuComponent> menuModel, GameState *gameState)
{
    _itemViews.clear();

    if (gameState == nullptr) {
        for (const auto& item : menuModel->getChildrens()) {
            if (item){
                Vector2 position = { 100, static_cast<float>(_itemViews.size() * 50 + 100) }; // Example positioning
                Vector2 size = { 200, 40 }; // Example size
                auto itemView = std::make_shared<MenuItemView>(position, size);
                itemView->setFont(ResourceManager::getInstance().getFont("GozaruDemo"));
                _itemViews.push_back(itemView);
            }
        }
    }
    else {
        _itemViews.clear();
        auto itemViews = gameState->createNavigationMenuButtonItemViews(menuModel);
        for (const auto& itemView : itemViews) {
            if (itemView) {
                _itemViews.push_back(itemView);
            }
        }
    }
}

void ButtonMenuView::render(std::shared_ptr<MenuComponent> menuModel) const
{
    const auto& menuItems = menuModel->getChildrens();
    for (size_t i = 0; i < _itemViews.size() && i < menuItems.size(); ++i) {
        const auto& itemView = _itemViews[i];
        const auto& menuItem = menuItems[i];
        if (itemView && menuItem && menuItem->isEnabled()) {
            itemView->render(menuItem);
        }
    }
}

void ButtonMenuView::createSettingMenuItemViews(int numberOfItems)
{
    _itemViews.clear();
    for (int i = 0; i < numberOfItems; ++i) {
        Vector2 position = { 100, static_cast<float>(i * 50 + 100) }; // Example positioning
        Vector2 size = { 200, 40 }; // Example size
        auto itemView = std::make_shared<MenuItemView>(position, size);
        _itemViews.push_back(itemView);
    }
}

void ButtonMenuView::createInGameItemsViews(int numberOfItems)
{
    _itemViews.clear();
    for (int i = 0; i < numberOfItems; ++i) {
        Vector2 position = { 100, static_cast<float>(i * 50 + 100) }; // Example positioning
        Vector2 size = { 200, 40 }; // Example size
        auto itemView = std::make_shared<MenuItemView>(position, size);
        _itemViews.push_back(itemView);
    }
}