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

void ButtonMenuView::createSettingsItemsViews(int numberOfItems)
{
    _itemViews.clear(); // Clear existing item views

    const float horizontalSpacing = 20.0f; // spacing between items
    const float itemHeight = 30.0f;
    const float itemWidth = 150.0f;
    const Rectangle menuArea = {0, 0, (float)GetScreenWidth(), 50.0f}; // Example menu area

    const float startX = menuArea.x + (menuArea.width - numberOfItems * itemWidth - (numberOfItems - 1) * horizontalSpacing) / 2;
    const float startY = menuArea.y + (menuArea.height - itemHeight) / 2;

    _itemViews.reserve(numberOfItems); // Reserve space for the specified number of items
    for (int i = 0; i < numberOfItems; ++i) {
        Vector2 position = {startX + i * (itemWidth + horizontalSpacing), startY};
        Vector2 size = {itemWidth, itemHeight};
        auto itemView = std::make_shared<MenuItemView>(position, size);
        itemView->setFont(ResourceManager::getInstance().getFont("GozaruDemo"));
        _itemViews.push_back(itemView);
    }
}

void ButtonMenuView::createInGameItemsViews(int numberOfItems)
{
    _itemViews.clear(); // Clear existing item views

    const float horizontalSpacing = 20.0f; // spacing between items
    const float itemHeight = 30.0f;
    const float itemWidth = 150.0f;
    const Rectangle menuArea = {0, 0, (float)GetScreenWidth(), 50.0f}; // Example menu area

    const float startX = menuArea.x + (menuArea.width - numberOfItems * itemWidth - (numberOfItems - 1) * horizontalSpacing) / 2;
    const float startY = menuArea.y + (menuArea.height - itemHeight) / 2;

    _itemViews.reserve(numberOfItems); // Reserve space for the specified number of items
    for (int i = 0; i < numberOfItems; ++i) {
        Vector2 position = {startX + i * (itemWidth + horizontalSpacing), startY};
        Vector2 size = {itemWidth, itemHeight};
        auto itemView = std::make_shared<MenuItemView>(position, size);
        itemView->setFont(ResourceManager::getInstance().getFont("GozaruDemo"));
        _itemViews.push_back(itemView);
    }
}

void ButtonMenuView::createSavedGameItemsViews(int numberOfItems)
{
    _itemViews.clear(); // Clear existing item views

    const float horizontalSpacing = 20.0f; // spacing between items
    const float itemHeight = 30.0f;
    const float itemWidth = 150.0f;
    const Rectangle menuArea = {100, 600, (float)GetScreenWidth() - 200.0f, 100.0f}; // Example menu area

    const float startX = menuArea.x + (menuArea.width - numberOfItems * itemWidth - (numberOfItems - 1) * horizontalSpacing) / 2;
    const float startY = menuArea.y + (menuArea.height - itemHeight) / 2;

    _itemViews.reserve(numberOfItems); // Reserve space for the specified number of items
    for (int i = 0; i < numberOfItems; ++i) {
        Vector2 position = {startX + i * (itemWidth + horizontalSpacing), startY};
        Vector2 size = {itemWidth, itemHeight};
        auto itemView = std::make_shared<MenuItemView>(position, size);
        itemView->setFont(ResourceManager::getInstance().getFont("GozaruDemo"));
        _itemViews.push_back(itemView);
    }
}

void ButtonMenuView::createPassButton(int numberOfItems)
{
    _itemViews.clear(); // Clear existing item views

    const float horizontalSpacing = 20.0f; // spacing between items
    const float itemHeight = 50.0f;
    const float itemWidth = 100.0f;
    const Rectangle menuArea = {400, 500, (float)GetScreenWidth(), 50.0f}; // Example menu area

    const float startX = menuArea.x + (menuArea.width - numberOfItems * itemWidth - (numberOfItems - 1) * horizontalSpacing) / 2;
    const float startY = menuArea.y + (menuArea.height - itemHeight) / 2;

    _itemViews.reserve(numberOfItems); // Reserve space for the specified number of items
    for (int i = 0; i < numberOfItems; ++i) {
        Vector2 position = {startX + i * (itemWidth + horizontalSpacing), startY};
        Vector2 size = {itemWidth, itemHeight};
        auto itemView = std::make_shared<MenuItemView>(position, size);
        itemView->setFont(ResourceManager::getInstance().getFont("GozaruDemo"));
        _itemViews.push_back(itemView);
    }
}
