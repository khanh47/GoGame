#include "GameLevelMenuController.h"
#include "GameLevelScene.h"
#include "MenuView.h"
#include "MenuComponent.h"
#include "Menu.h"
#include "MenuItem.h"
#include "ListMenuView.h"
#include "MenuCommand.h"
#include <cmath>
#include <algorithm>

GameLevelMenuController::GameLevelMenuController(GameLevelScene* GameLevelScene)
    : _GameLevelScene(GameLevelScene) {
    _menuView = nullptr;
}

void GameLevelMenuController::setViewStrategy(std::unique_ptr<IMenuView> view) {
    _menuView = std::move(view);
    if (_menuView && _menuSystem) {
        _menuView->createInGameItemsViews(_menuSystem->getChildrens().size());
    }
}

void GameLevelMenuController::handleInput() {
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
                    itemViews[i]->setSelected(static_cast<int>(i) == _selectedGameLevelIndex);
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

            // Set selected state based on the currently selected game level index
            itemViews[i]->setSelected(static_cast<int>(i) == _selectedGameLevelIndex);

            if (isHovered && mouseClicked) {
                // Update selected game level index
                _selectedGameLevelIndex = static_cast<int>(i);

                // Execute the command for this menu item
                auto command = menuItems[i]->cloneCommand();
                if (command) {
                    command->execute();
                }
            }
        }
    }
}

void GameLevelMenuController::update() {
    // Update scrollbar if using ListMenuView
    if (_menuView) {
        if (auto* listView = dynamic_cast<ListMenuView*>(_menuView.get())) {
            listView->updateScrollbar();
        }

        // Update selected state for all menu items
        const auto& itemViews = _menuView->getItemViews();
        for (size_t i = 0; i < itemViews.size(); ++i) {
            if (itemViews[i]) {
                itemViews[i]->setSelected(static_cast<int>(i) == _selectedGameLevelIndex);
            }
        }
    }
}

void GameLevelMenuController::render() const {
    if (_menuView && _menuSystem) {
        _menuView->render(_menuSystem);
    }
}

void GameLevelMenuController::createGameLevelMenu() {
    _menuSystem = std::make_shared<Menu>("Game Mode selection", true);

    auto Easy = std::make_shared<MenuItem>("EASY", true);
    Easy->setCommand(std::make_unique<GameLevelSelectCommand>("EASY", _GameLevelScene));
    _menuSystem->addItem(Easy);

    auto Medium = std::make_shared<MenuItem>("MEDIUM", true);
    Medium->setCommand(std::make_unique<GameLevelSelectCommand>("MEDIUM", _GameLevelScene));
    _menuSystem->addItem(Medium);

    auto Hard = std::make_shared<MenuItem>("HARD", true);
    Hard->setCommand(std::make_unique<GameLevelSelectCommand>("HARD", _GameLevelScene));
    _menuSystem->addItem(Hard);

    if (_menuView) {
        _menuView->createInGameItemsViews(_menuSystem->getChildrens().size());
    }
}

void GameLevelMenuController::selectGameLevel(const std::string &level) {
    _selectedGameLevel = level;

    // Find selected game level index
    if (_menuSystem) {
        const auto& menuItem = _menuSystem->getChildrens();
        for (size_t i = 0; i < menuItem.size(); ++i) {
            if (menuItem[i]->getTitle() == level) {
                _selectedGameLevelIndex = static_cast<int>(i);
                break;
            }
        }
    }
}
