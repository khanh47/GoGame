#include "GameState.h"
#include "ConcreteGameStates/GameDataState.h"
#include "ResourceManager.h"
#include "GameDataScene.h"
#include "MenuItemView.h"
#include "MenuComponent.h"
#include "SceneManager.h"
#include "MenuCommand.h"
#include "MenuItem.h"
#include "Menu.h"
#include "GameDataScene.h"
#include "GameController.h"

void GameDataState::onEnter(GameStateModel *context)
{
}

void GameDataState::onExit(GameStateModel *context)
{
}

void GameDataState::update(GameStateModel *context, float deltaTime)
{
}

std::unique_ptr<GameState> GameDataState::clone() const
{
		return std::make_unique<GameDataState>();
}

std::unique_ptr<Scene> GameDataState::createScene() const
{
    return std::make_unique<GameDataScene>();
}

std::shared_ptr<MenuComponent> GameDataState::createNavigationMenu(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    auto gameDataMenu = std::make_shared<Menu>("Load Game Menu", true);
    auto Back = std::make_shared<MenuItem>("BACK", true);
		Back->setCommand(createGameModeBackCommand(gameStateModel, sceneManager));
    gameDataMenu->addItem(Back);

    if (gameDataSelected) {
        auto Play = std::make_shared<MenuItem>("PLAY", true);
				Play->setCommand(createPlayCommand(gameStateModel, sceneManager, selectedGameData));
        gameDataMenu->addItem(Play);
    }

    return gameDataMenu;
}

std::vector<std::shared_ptr<MenuItemView>> GameDataState::createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const
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

void GameDataState::setGameDataSelected(bool selected, const std::string &data)
{
    gameDataSelected = selected;
    if (selected && !data.empty()) {
				selectedGameData = data;
    } else {
        selectedGameData = "NONE";
    }
    ++menuVersion; // Increment menu version to force menu refresh
}

