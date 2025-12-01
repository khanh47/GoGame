#include "InGameMenuController.h"
#include "InGameScene.h"
#include "MenuView.h"
#include "Menu.h"
#include "MenuItem.h"
#include "GameCommand.h"
#include "SavedGameCommand.h"
#include "GameController.h"

#include <iostream>
#include <cmath>

InGameMenuController::InGameMenuController(InGameScene* inGameScene, GameController* gameModel)
    : _inGameScene(inGameScene), _gameController(gameModel) {
    _menuView = nullptr;
}

void InGameMenuController::setViewStrategy(std::unique_ptr<IMenuView> view) {
    _menuView = std::move(view);
    if (_menuView && _menuSystem) {
        _menuView->createInGameItemsViews(_menuSystem->getChildrens().size());
    }
}

void InGameMenuController::handleInput() {
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

void InGameMenuController::update() {
}

void InGameMenuController::render() const {
    if (_menuView && _menuSystem) {
        _menuView->render(_menuSystem);
    }
}

void InGameMenuController::createInGameMenu() {
    _menuSystem = std::make_shared<Menu>("Game Data selection", true);

		auto Reset = std::make_shared<MenuItem>("RESET", true);
		Reset->setCommand(std::make_unique<ResetCommand>(_gameController));
		_menuSystem->addItem(Reset);

		auto Undo = std::make_shared<MenuItem>("UNDO", true);
		Undo->setCommand(std::make_unique<UndoCommand>(_gameController));
		_menuSystem->addItem(Undo);

		auto Redo = std::make_shared<MenuItem>("REDO", true);
		Redo->setCommand(std::make_unique<RedoCommand>(_gameController));
		_menuSystem->addItem(Redo);

		auto Save = std::make_shared<MenuItem>("SAVE", true);
		Save->setCommand(createOpenSavedListCommand(_inGameScene));
		_menuSystem->addItem(Save);

    if (_menuView) {
        _menuView->createInGameItemsViews(_menuSystem->getChildrens().size());
    }
}
