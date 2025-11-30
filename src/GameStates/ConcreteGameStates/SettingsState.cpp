#include "GameState.h"
#include "SettingsState.h"
#include "SettingsScene.h"
#include "Scene.h"
#include "MenuComponent.h"
#include "Menu.h"
#include "SceneManager.h"
#include "MenuItemView.h"
#include "ResourceManager.h"
#include "MenuCommand.h"
#include "MenuItem.h"
#include <iostream>

void SettingsState::onEnter(GameStateModel *context)
{
    std::cout << "Entering Settings State" << std::endl;
}

void SettingsState::onExit(GameStateModel *context)
{
    std::cout << "Exiting Settings State" << std::endl;
}

void SettingsState::update(GameStateModel *context, float deltaTime)
{
}

std::unique_ptr<GameState> SettingsState::clone() const
{
    return std::make_unique<SettingsState>();
}

std::unique_ptr<Scene> SettingsState::createScene() const
{
    return std::make_unique<SettingsScene>();
}

std::shared_ptr<MenuComponent> SettingsState::createNavigationMenu(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    auto settingsMenu = std::make_shared<Menu>("Settings Menu", true);
		auto Back = std::make_shared<MenuItem>("BACK", true);
		Back->setCommand(createGameModeBackCommand(gameStateModel, sceneManager));

		settingsMenu->addItem(Back);
    return settingsMenu;
}

std::vector<std::shared_ptr<MenuItemView>> SettingsState::createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const
{
    std::vector<std::shared_ptr<MenuItemView>> itemViews;

    int activeItems = 0;
    for (const auto& child : menu->getChildrens()) {
        if (child) {
            ++activeItems;
        }
    }

    const float verticalSpacing = 20.0f; // spacing between items
    const float itemHeight = 30.0f;
    const float itemWidth = 150.0f;
    const Rectangle menuArea = {0, 0, 250, 50}; // Example menu area

    const float startX = menuArea.x + (menuArea.width - itemWidth) / 2;
    const float startY = menuArea.y + (menuArea.height - (activeItems * itemHeight + (activeItems - 1) * verticalSpacing)) / 2;

    itemViews.reserve(activeItems); // Reserve space for active items
    for (int i = 0; i < activeItems; ++i) {
        Vector2 position = {startX, startY + i * (itemHeight + verticalSpacing)};
        Vector2 size = {itemWidth, itemHeight};
        auto itemView = std::make_shared<MenuItemView>(position, size);
        itemView->setFont(ResourceManager::getInstance().getFont("GozaruDemo"));
        itemViews.push_back(itemView);
    }
    return itemViews;
}
