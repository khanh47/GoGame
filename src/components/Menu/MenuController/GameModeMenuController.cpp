#include "GameModeMenuController.h"
#include "ConcreteScene/GameModeScene.h"
#include "MenuView.h"
#include "MenuComponent.h"
#include "Menu.h"
#include "MenuItem.h"
#include "ListMenuView.h"
#include "MenuCommand.h"
#include <cmath>
#include <algorithm>

GameModeMenuController::GameModeMenuController(GameModeScene* gameModeScene)
    : _gameModeScene(gameModeScene) {
    _menuView = nullptr;
}

void GameModeMenuController::setViewStrategy(std::unique_ptr<IMenuView> view) {
    _menuView = std::move(view);
    if (_menuView && _menuSystem) {
        _menuView->createInGameItemsViews(_menuSystem->getChildrens().size());
    }
}

void GameModeMenuController::handleInput() {
    if (!_menuView || !_menuSystem) return;

    Vector2 mousePos = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    // Handle scroll input for ListMenuView
    if (auto* listView = dynamic_cast<ListMenuView*>(_menuView.get())) {
        listView->handleScrollInput();
    }

    // Handle hover states and clicks
    const auto& itemViews = _menuView->getItemViews();
    const auto& menuItems = _menuSystem->getChildrens();

    for (size_t i = 0; i < itemViews.size() && i < menuItems.size(); ++i) {
        if (itemViews[i] && menuItems[i]->isEnabled()) {
            Vector2 itemPos, itemSize;
            Rectangle itemRect;

            // Get correct position accounting for scroll offset in ListMenuView
            if (auto* listView = dynamic_cast<ListMenuView*>(_menuView.get())) {
                itemPos = listView->getScrolledItemPosition(i);
                itemSize = itemViews[i]->getSize();

                // Check if item is visible in the list area
                Rectangle listArea = listView->getListArea();
                if (itemPos.y + itemSize.y < listArea.y || itemPos.y > listArea.y + listArea.height) {
                    // Item is not visible, skip interaction
                    itemViews[i]->setHovered(false);
                    itemViews[i]->setSelected(static_cast<int>(i) == _selectedGameModeIndex);
                    continue;
                }

                // Ensure click area doesn't extend beyond the list area (excluding scrollbar)
                Rectangle clickableArea = {
                    fmaxf(itemPos.x, listArea.x),
                    fmaxf(itemPos.y, listArea.y),
                    fminf(itemSize.x, listArea.width - 15.0f), // Account for scrollbar width
                    fminf(itemSize.y, listArea.y + listArea.height - itemPos.y)
                };
                itemRect = clickableArea;
            } else {
                itemPos = itemViews[i]->getPosition();
                itemSize = itemViews[i]->getSize();
                itemRect = {itemPos.x, itemPos.y, itemSize.x, itemSize.y};
            }

            bool isHovered = CheckCollisionPointRec(mousePos, itemRect);

            itemViews[i]->setHovered(isHovered);

            // Set selected state based on the currently selected game mode index
            itemViews[i]->setSelected(static_cast<int>(i) == _selectedGameModeIndex);

            if (isHovered && mouseClicked) {
                // Update selected game mode index
                _selectedGameModeIndex = static_cast<int>(i);

                // Execute the command for this menu item
                auto command = menuItems[i]->cloneCommand();
                if (command) {
                    command->execute();
                }
            }
        }
    }
}

void GameModeMenuController::update() { 
    // Update scrollbar if using ListMenuView
    if (_menuView) {
        if (auto* listView = dynamic_cast<ListMenuView*>(_menuView.get())) {
            listView->updateScrollbar();
        }

        // Update selected state for all menu items
        const auto& itemViews = _menuView->getItemViews();
        for (size_t i = 0; i < itemViews.size(); ++i) {
            if (itemViews[i]) {
                itemViews[i]->setSelected(static_cast<int>(i) == _selectedGameModeIndex);
            }
        }
    }
}

void GameModeMenuController::render() const {
    if (_menuView && _menuSystem) {
        _menuView->render(_menuSystem);
    }
}

void GameModeMenuController::createGameModeMenu() {
    _menuSystem = std::make_shared<Menu>("Game Mode selection", true);

    auto playerVsPlayer = std::make_shared<MenuItem>("PLAYER VS PLAYER", true);
    auto playerVsBot = std::make_shared<MenuItem>("PLAYER VS BOT", true);

    auto command = std::make_unique<GameModeSelectCommand>("PLAYER VS PLAYER", _gameModeScene);
    playerVsPlayer->setCommand(std::move(command));
    command = std::make_unique<GameModeSelectCommand>("PLAYER VS BOT", _gameModeScene);
    playerVsBot->setCommand(std::move(command));

    _menuSystem->addItem(playerVsPlayer);
    _menuSystem->addItem(playerVsBot);

    if (_menuView) {
        _menuView->createInGameItemsViews(_menuSystem->getChildrens().size());
    }
}

void GameModeMenuController::selectGameMode(const std::string &mode) {
    _selectedGameMode = mode;

    // Find selected game mode index
    if (_menuSystem) {
        const auto& menuItem = _menuSystem->getChildrens();
        for (size_t i = 0; i < menuItem.size(); ++i) {
            if (menuItem[i]->getTitle() == mode) {
                _selectedGameModeIndex = static_cast<int>(i);
                break;
            }
        }
    }
}
