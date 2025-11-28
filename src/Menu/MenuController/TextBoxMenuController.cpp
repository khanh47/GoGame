#include "TextBoxMenuController.h"
#include "SavedGameCommand.h"
#include "InGameScene.h"
#include "MenuView.h"
#include "MenuComponent.h"
#include "Menu.h"
#include "MenuItem.h"
#include "ButtonMenuView.h"
#include <iostream>

TextBoxMenuController::TextBoxMenuController(InGameScene* inGameScene)
    : _inGameScene(inGameScene) {
    _menuView = nullptr;
}

void TextBoxMenuController::setViewStrategy(std::unique_ptr<IMenuView> view) {
    _menuView = std::move(view);
    if (_menuView && _menuSystem) {
        _menuView->createSavedGameItemsViews(_menuSystem->getChildrens().size());
    }
}

void TextBoxMenuController::handleInput() {
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

void TextBoxMenuController::update() {
}

void TextBoxMenuController::render() const {
    if (_menuView && _menuSystem) {
        _menuView->render(_menuSystem);
    }
}

void TextBoxMenuController::createMenu() {
    _menuSystem = std::make_shared<Menu>("Game Data selection", true);

    auto btnBack = std::make_shared<MenuItem>("BACK", true);
		btnBack->setCommand(createCloseCreateInputCommand(_inGameScene));
		_menuSystem->addItem(btnBack);

    auto btnEnter = std::make_shared<MenuItem>("ENTER", true);
		btnEnter->setCommand(createCloseAndCreateInputCommand(_inGameScene));
		_menuSystem->addItem(btnEnter);

    if (_menuView) {
        _menuView->createSavedGameItemsViews(_menuSystem->getChildrens().size());
    }
}
