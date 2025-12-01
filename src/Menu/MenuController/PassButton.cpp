#include "PassButton.h"
#include "ButtonMenuView.h"
#include "GameCommand.h"
#include "GameController.h"
#include "Menu.h"
#include "MenuComponent.h"
#include "MenuItem.h"
#include "MenuView.h"
#include <iostream>

PassButton::PassButton(GameController *gameModel) : _gameController(gameModel) { _menuView = nullptr; }

void PassButton::setViewStrategy(std::unique_ptr<IMenuView> view) {
	_menuView = std::move(view);
	if (_menuView && _menuSystem) {
		_menuView->createPassButton(_menuSystem->getChildrens().size());
	}
}

void PassButton::handleInput() {
	if (!_menuView || !_menuSystem)
		return;
	Vector2 mousePosition = GetMousePosition();
	bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

	if (!_menuView) {
		std::cout << "Menu view is not set!" << std::endl;
		return;
	}

	const std::vector<std::shared_ptr<MenuItemView>> &itemViews = _menuView->getItemViews();
	const auto &menuItems = _menuSystem->getChildrens();

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

void PassButton::update() {}

void PassButton::render() const {
	if (_menuView && _menuSystem) {
		_menuView->render(_menuSystem);
	}
}

void PassButton::createPassButton() {
	_menuSystem = std::make_shared<Menu>("Pass Game Button", true);

	auto Pass = std::make_shared<MenuItem>("PASS", true);
	Pass->setCommand(std::make_unique<PassCommand>(_gameController));
	_menuSystem->addItem(Pass);

	if (_menuView) {
		_menuView->createPassButton(_menuSystem->getChildrens().size());
	}
}
