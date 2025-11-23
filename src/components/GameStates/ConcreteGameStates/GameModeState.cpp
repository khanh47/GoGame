#include "GameState.h"
#include "ConcreteGameStates/GameModeState.h"
#include "ResourceManager.h"
#include "GameModeScene.h"
#include "MenuItemView.h"
#include "MenuComponent.h"
#include "SceneManager.h"
#include "MenuCommand.h"
#include "MenuItem.h"
#include "Menu.h"
#include "ConcreteGameStates/GameModeState.h"

void GameModeState::onEnter(GameStateModel *context)
{
}

void GameModeState::onExit(GameStateModel *context)
{
}

void GameModeState::update(GameStateModel *context, float deltaTime)
{
}

std::unique_ptr<GameState> GameModeState::clone() const
{
    auto cloned = std::make_unique<GameModeState>();

    cloned->gameModeSelected = this->gameModeSelected;
    cloned->selectedGameMode = this->selectedGameMode;
    cloned->menuVersion = this->menuVersion;
    return cloned;
}

std::unique_ptr<Scene> GameModeState::createScene() const
{
    return std::make_unique<GameModeScene>();
}

std::shared_ptr<MenuComponent> GameModeState::createNavigationMenu(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    auto gameModeMenu = std::make_shared<Menu>("Game Mode Menu", true);
    auto Back = std::make_shared<MenuItem>("BACK", true);
    Back->setCommand(createGameModeBackCommand(gameStateModel, sceneManager));
    gameModeMenu->addItem(Back);

    if (gameModeSelected) {
        auto Play = std::make_shared<MenuItem>("PLAY", true);
        Play->setCommand(createPlayCommand(gameStateModel, sceneManager, selectedGameMode));
        gameModeMenu->addItem(Play);
    }

    return gameModeMenu;
}

std::vector<std::shared_ptr<MenuItemView>> GameModeState::createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const
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
    const float startY = 400.0f;

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

void GameModeState::setGameModeSelected(bool selected, const std::string &mode)
{
    gameModeSelected = selected;
    if (selected && !mode.empty()) {
        selectedGameMode = mode;
    } else {
        selectedGameMode = "NONE";
    }
    ++menuVersion; // Increment menu version to force menu refresh
}
