#include "SettingsMenuController.h"
#include "SettingsScene.h"
#include "MenuView.h"
#include "Menu.h"
#include "MenuItem.h"

#include <iostream>
#include <cmath>

SettingsMenuController::SettingsMenuController(SettingsScene* settingsScene)
    : _settingsScene(settingsScene) {
    _menuView = nullptr;
}

void SettingsMenuController::setViewStrategy(std::unique_ptr<IMenuView> view) {
    _menuView = std::move(view);
    if (_menuView && _menuSystem) {
        _menuView->createSettingsItemsViews(_menuSystem->getChildrens().size());
    }
}

void SettingsMenuController::handleInput() {
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

void SettingsMenuController::update() {
}

void SettingsMenuController::render() const {
  if (_menuView && _menuSystem) {
    _menuView->render(_menuSystem);
  }
}

void SettingsMenuController::createSettingsMenu() {
  _menuSystem = std::make_shared<Menu>("Settings selection", true);

	auto Sound = std::make_shared<MenuItem>("SOUND", true);
	_menuSystem->addItem(Sound);

	auto Theme = std::make_shared<MenuItem>("THEME", true);
	_menuSystem->addItem(Theme);

  if (_menuView) {
		_menuView->createSettingsItemsViews(_menuSystem->getChildrens().size());
  }
}
