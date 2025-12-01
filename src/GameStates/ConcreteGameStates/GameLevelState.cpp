#include "GameState.h"
#include "ConcreteGameStates/GameLevelState.h"
#include "ResourceManager.h"
#include "GameLevelScene.h"
#include "MenuItemView.h"
#include "MenuComponent.h"
#include "SceneManager.h"
#include "MenuCommand.h"
#include "MenuItem.h"
#include "Menu.h"
#include "ConcreteGameStates/GameLevelState.h"

void GameLevelState::onEnter(GameStateModel *context)
{
}

void GameLevelState::onExit(GameStateModel *context)
{
}

void GameLevelState::update(GameStateModel *context, float deltaTime)
{
}

std::unique_ptr<GameState> GameLevelState::clone() const
{
    auto cloned = std::make_unique<GameLevelState>();

    cloned->gameLevelSelected = this->gameLevelSelected;
    cloned->selectedGameLevel = this->selectedGameLevel;
    cloned->menuVersion = this->menuVersion;
    return cloned;
}

std::unique_ptr<Scene> GameLevelState::createScene() const
{
    return std::make_unique<GameLevelScene>();
}

std::shared_ptr<MenuComponent> GameLevelState::createNavigationMenu(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    std::cout << "Create game level menu controller with: " << selectedGameLevel << std::endl;
    auto GameLevelMenu = std::make_shared<Menu>("Game Level Menu", true);
    auto Back = std::make_shared<MenuItem>("BACK", true);
    Back->setCommand(createGameLevelBackCommand(gameStateModel, sceneManager));
    GameLevelMenu->addItem(Back);

    if (gameLevelSelected) {
        auto Play = std::make_shared<MenuItem>("PLAY", true);
        Play->setCommand(createPlayCommand(gameStateModel, sceneManager, selectedGameLevel));
        GameLevelMenu->addItem(Play);
    }

    return GameLevelMenu;
}

std::vector<std::shared_ptr<MenuItemView>> GameLevelState::createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const
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

void GameLevelState::setGameLevelSelected(bool selected, const std::string &level)
{
    gameLevelSelected = selected;
    if (selected && !level.empty()) {
        selectedGameLevel = level;
    } else {
        selectedGameLevel = "NONE";
    }
    ++menuVersion; // Increment menu version to force menu refresh
}
