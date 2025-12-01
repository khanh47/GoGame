#include "InGameState.h"
#include "GameController.h"
#include "InGameScene.h"
#include "Menu.h"
#include "MenuCommand.h"
#include "MenuComponent.h"
#include "MenuItem.h"
#include "MenuItemView.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"

void InGameState::onEnter(GameStateModel *context) {}

void InGameState::onExit(GameStateModel *context) {}

void InGameState::update(GameStateModel *context, float deltaTime) {
	// Update game logic here
}

std::unique_ptr<GameState> InGameState::clone() const { return std::make_unique<InGameState>(_gameModeSelected); }

std::unique_ptr<Scene> InGameState::createScene() const { return std::make_unique<InGameScene>(_gameModeSelected); }

std::shared_ptr<MenuComponent> InGameState::createNavigationMenu(GameStateModel *gameStateModel,
																																 SceneManager *sceneManager) {
	auto inGameMenu = std::make_shared<Menu>("In-Game Menu", sceneManager);

	auto Back = std::make_shared<MenuItem>("BACK", true);
	Back->setCommand(createInGameBackCommand(gameStateModel, sceneManager));
	inGameMenu->addItem(Back);

	return inGameMenu;
}

std::vector<std::shared_ptr<MenuItemView>>
InGameState::createNavigationMenuButtonItemViews(std::shared_ptr<MenuComponent> menu) const {
	std::vector<std::shared_ptr<MenuItemView>> itemViews;

	int activeItems = 0;
	for (const auto &child : menu->getChildrens()) {
		if (child) {
			++activeItems;
		}
	}

	const float verticalSpacing = 20.0f; // spacing between items
	const float itemHeight = 30.0f;
	const float itemWidth = 150.0f;
	const Rectangle menuArea = {0, 0, 250, 50}; // Example menu area

	const float startX = menuArea.x + (menuArea.width - itemWidth) / 2;
	const float startY =
			menuArea.y + (menuArea.height - (activeItems * itemHeight + (activeItems - 1) * verticalSpacing)) / 2;

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
