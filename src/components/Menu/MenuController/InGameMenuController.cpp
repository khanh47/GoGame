#include "InGameMenuController.h"
#include "ConcreteScene/InGameScene.h"
#include "MenuView.h"
#include "MenuComponent.h"

InGameMenuController::InGameMenuController(InGameScene* inGameScene)
    : _inGameScene(inGameScene) {
    _menuView = nullptr;
}

void InGameMenuController::setViewStrategy(std::unique_ptr<IMenuView> view) {
    _menuView = std::move(view);
    if (_menuView && _menuSystem) {
        _menuView->createInGameItemsViews(_menuSystem->getChildrens().size());
    }
}

void InGameMenuController::handleInput() {
    Vector2 mousePosition = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (!_menuView) {
        std::cout << "InGameMenuView is not set" << std::endl;
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
            }
        }
    }
}

void InGameMenuController::update() { 

}

void InGameMenuController::render() const {
    if (_menuView && _menuSystem) {
        _menuView->render(_menuSystem);
    } else {
        std::cerr << "InGameMenuView is not initialized";
    }
}

void InGameMenuController::createGameModeMenu() {
}

void InGameMenuController::selectGameMode(const std::string &mode) {
}
