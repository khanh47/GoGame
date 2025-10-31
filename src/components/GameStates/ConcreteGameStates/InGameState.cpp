#include "GameState.h"
#include "ConcreteGameStates/InGameState.h"
#include "ResourceManager.h"
#include "InGameScene.h"
#include "MenuItemView.h"
#include "MenuComponent.h"
#include "SceneManager.h"
#include "MenuItem.h"
#include "Menu.h"
#include "ConcreteGameStates/InGameState.h"

void InGameState::onEnter(GameStateModel *context)
{
}

void InGameState::onExit(GameStateModel *context)
{
}

void InGameState::update(GameStateModel *context, float deltaTime)
{
}

std::unique_ptr<GameState> InGameState::clone() const
{
    return std::make_unique<InGameState>();
}

std::unique_ptr<Scene> InGameState::createScene() const
{
    return std::make_unique<InGameScene>();
}

std::shared_ptr<MenuComponent> InGameState::createNavigationMenu(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    auto inGameMenu = std::make_shared<Menu>("In-Game Menu", true);
    std::shared_ptr<MenuComponent> Pass = std::make_shared<MenuItem>("Pass", true);
    inGameMenu->addItem(Pass);
    return inGameMenu;
}

std::vector<std::shared_ptr<MenuItemView>> InGameState::createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const
{
    std::vector<std::shared_ptr<MenuItemView>> itemViews;

    int activeItems = 0;
    for (const auto& child : menu->getChildrens()) {
        if (child) {
            ++activeItems;
        }
    }
   
    const float horizontalSpacing = 20.0f; // spacing between items
    const float itemHeight = 40.0f;
    const float itemWidth = 200.0f;
    const float startX = (GetScreenWidth() - activeItems * itemWidth + (activeItems - 1) * horizontalSpacing) / 2.0f;
    const float startY = 650.0f;

    itemViews.reserve(activeItems); // Reserve space for active items
    for (int i = 0; i < activeItems; ++i) {
        Vector2 position = {startX + i * (itemWidth + horizontalSpacing), startY};
        Vector2 size = {itemWidth, itemHeight};
        auto itemView = std::make_shared<MenuItemView>(position, size);
        itemView->setFont(ResourceManager::getInstance().getFont("GozaruDemo"));
        itemViews.push_back(itemView);
    }
    return itemViews;
}

void InGameState::setGameModeSelected(bool selected, const std::string &mode)
{
    gameModeSelected = selected;
    if (selected && !mode.empty()) {
        selectedGameMode = mode;
    } else {
        selectedGameMode = "NONE";
    }
    ++menuVersion; // Increment menu version to force menu refresh
}