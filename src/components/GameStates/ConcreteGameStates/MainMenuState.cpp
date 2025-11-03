#include "GameState.h"
#include "ConcreteGameStates/MainMenuState.h"
#include "ConcreteScene/MainMenuScene.h"
#include "Scene.h"
#include "MenuComponent.h"
#include "Menu.h"
#include "SceneManager.h"
#include "MenuItemView.h"
#include "ResourceManager.h"
#include "MenuCommand.h"
#include "MenuItem.h"
#include <iostream>

void MainMenuState::onEnter(GameStateModel *context)
{
    std::cout << "Entering Main Menu State" << std::endl;
}

void MainMenuState::onExit(GameStateModel *context)
{
    std::cout << "Exiting Main Menu State" << std::endl;
}

void MainMenuState::update(GameStateModel *context, float deltaTime)
{
}

std::unique_ptr<GameState> MainMenuState::clone() const
{
    return std::make_unique<MainMenuState>();
}

std::unique_ptr<Scene> MainMenuState::createScene() const
{
    return std::make_unique<MainMenuScene>();
}

std::shared_ptr<MenuComponent> MainMenuState::createNavigationMenu(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    auto mainMenu = std::make_shared<Menu>("Main Menu", true);

    std::shared_ptr<MenuComponent> NewGame = std::make_shared<MenuItem>("NEW GAME", true);
    NewGame->setCommand(createNewGameCommand(gameStateModel, sceneManager));

    std::shared_ptr<MenuComponent> Exit = std::make_shared<MenuItem>("EXIT", true);
    Exit->setCommand(createExitCommand());

    mainMenu->addItem(NewGame);
    mainMenu->addItem(Exit);
    return mainMenu;
}

std::vector<std::shared_ptr<MenuItemView>> MainMenuState::createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const
{
    std::vector<std::shared_ptr<MenuItemView>> itemViews;

    int activeItems = 0;
    for (const auto &child : menu->getChildrens())
    {
        if (child)
        {
            ++activeItems;
        }
    }

    const float verticalSpacing = 20.0f; // spacing between items
    const float itemHeight = 60.0f;
    const float itemWidth = 240.0f;
    const Rectangle menuArea = {0, 0, 300, (float)GetScreenHeight()}; // Example menu area
    std::cout << menuArea.x << std::endl;
    const float startX = menuArea.x + (menuArea.width - itemWidth) / 2;
    const float startY = menuArea.y + (menuArea.height - (activeItems * itemHeight + (activeItems - 1) * verticalSpacing)) / 2;

    itemViews.reserve(activeItems); // Reserve space for active items
    for (int i = 0; i < activeItems; ++i)
    {
        Vector2 position = {startX, startY + i * (itemHeight + verticalSpacing)};
        Vector2 size = {itemWidth, itemHeight};
        auto itemView = std::make_shared<MenuItemView>(position, size);
        itemView->setFont(ResourceManager::getInstance().getFont("GozaruDemo"));
        itemViews.push_back(itemView);
    }
    return itemViews;
}
