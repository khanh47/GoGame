#include "SavedGameMenuController.h"
#include "InGameScene.h"
#include "MenuView.h"
#include "Menu.h"
#include "MenuItem.h"
#include "SavedGameCommand.h"
#include "DataManager.h"
#include <iostream>

SavedGameMenuController::SavedGameMenuController(InGameScene* inGameScene, DataManager* dataManager)
    : _inGameScene(inGameScene), _dataManager(dataManager) {
    _menuView = nullptr;
}

void SavedGameMenuController::setViewStrategy(std::unique_ptr<IMenuView> view) {
    _menuView = std::move(view);
    if (_menuView && _menuSystem) {
        _menuView->createInGameItemsViews(_menuSystem->getChildrens().size());
    }
}

void SavedGameMenuController::handleInput() {
    if (!_menuView || !_menuSystem) return;
    Vector2 mousePosition = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (!_menuView){
        std::cout << "Menu view is not set!" << std::endl;
        return;
    }

    const std::vector<std::shared_ptr<MenuItemView>>& itemViews = _menuView->getItemViews();
    const auto& menuItems = _menuSystem->getChildrens();

    for (size_t i = 0; i < itemViews.size() && i < menuItems.size(); ++i) {
        bool isHovered = itemViews[i]->isPointed(mousePosition) && menuItems[i]->isEnabled();
        itemViews[i]->setHovered(isHovered);

        if (isHovered && mouseClicked) {
            auto command = menuItems[i]->cloneCommand();
            if (command) {
                command->execute();
                CommandType cmdType = command->getType();

                if (cmdType == CommandType::STATE_CHANGING || cmdType == CommandType::IMMEDIATE) {
                    break;
                }
            }
        }
    }
}

void SavedGameMenuController::update() {
}

void SavedGameMenuController::render() const {
    if (_menuView && _menuSystem) {
        _menuView->render(_menuSystem);
    }
}

void SavedGameMenuController::createSavedGameMenu() {
    _menuSystem = std::make_shared<Menu>("Game Data selection", true);

    auto btnBack = std::make_shared<MenuItem>("BACK", true);
		btnBack->setCommand(createCloseSavedListCommand(_inGameScene));
		_menuSystem->addItem(btnBack);

		auto btnDelete = std::make_shared<MenuItem>("DELETE", true);
    btnDelete->setCommand(createDeleteSelectedCommand(_dataManager));
    _menuSystem->addItem(btnDelete);

    auto btnCreate = std::make_shared<MenuItem>("CREATE", true);
    btnCreate->setCommand(createOpenCreateInputCommand(_inGameScene));
    _menuSystem->addItem(btnCreate);

		if (_dataManager->isGameDataSelected()) {
				auto btnSave = std::make_shared<MenuItem>("SAVE", true);
    		btnSave->setCommand(createSaveToSelectedCommand(_dataManager));
    		_menuSystem->addItem(btnSave);
		}

    if (_menuView) {
        _menuView->createSavedGameItemsViews(_menuSystem->getChildrens().size());
    }
}
