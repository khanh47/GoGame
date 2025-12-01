#include "MainMenuState.h"
#include "GameModeState.h"
#include "GameDataState.h"
#include "NavigationMenuController.h"
#include "ButtonMenuView.h"
#include "GameState.h"
#include "MenuComponent.h"
#include "MenuItemView.h"
#include "ICommand.h"
#include <memory>
#include <iostream>
#include <vector>

NavigationMenuController::NavigationMenuController(GameStateModel* gameStateModel, std::shared_ptr<MenuComponent> menuSystem, SceneManager* sceneManager)
    : _gameStateModel(gameStateModel), _lastState(std::make_unique<GameModeState>()), _sceneManager(sceneManager) {
    _menuSystem = menuSystem;
    _currentMenuModel = menuSystem;
    _menuView = std::make_unique<ButtonMenuView>();
}

void NavigationMenuController::setViewStrategy(std::unique_ptr<IMenuView> view) {
    _menuView = std::move(view);
    if (_menuView) {
        _menuView->createNavigationItemViews(_currentMenuModel, _gameStateModel->getCurrentState());
    }
}

void NavigationMenuController::updateNavigationMenuForCurrentState() {
    bool shouldUpdate = false;

    // check if state has changed
    if (_gameStateModel->getCurrentState()->getName() != _lastState->getName()) {
        shouldUpdate = true;
    }
    // special case for GameModeState to always update
    else if (auto* gameModeState = dynamic_cast<GameModeState*>(_gameStateModel->getCurrentState())) {
        if (auto* lastGameModeState = dynamic_cast<GameModeState*>(_lastState.get())) {
            if (gameModeState->getMenuVersion() != lastGameModeState->getMenuVersion()) {
                shouldUpdate = true;
            }
        } else {
            shouldUpdate = true;
        }
    }
    // special case for GameDataState to always update
    else if (auto* gameDataState = dynamic_cast<GameDataState*>(_gameStateModel->getCurrentState())) {
        if (auto* lastGameDataState = dynamic_cast<GameDataState*>(_lastState.get())) {
            if (gameDataState->getMenuVersion() != lastGameDataState->getMenuVersion()) {
                shouldUpdate = true;
            }
        } else {
            shouldUpdate = true;
        }
    }

    if (shouldUpdate) {
        std::shared_ptr<MenuComponent> newMenuModel = _gameStateModel->getCurrentState()->createNavigationMenu(_gameStateModel, _sceneManager);
        if (newMenuModel) {
            _currentMenuModel = newMenuModel;
            _menuView->createNavigationItemViews(_currentMenuModel, _gameStateModel->getCurrentState());
        }
        _lastState = std::move(_gameStateModel->getCurrentState()->clone());
    }
}

void NavigationMenuController::handleInput() {
    Vector2 mousePosition = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (!_menuView){
        std::cout << "Menu view is not set!" << std::endl;
        return;
    }

    const std::vector<std::shared_ptr<MenuItemView>>& itemViews = _menuView->getItemViews();
    const auto& menuItems = _currentMenuModel->getChildrens();

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

void NavigationMenuController::update() {
    updateNavigationMenuForCurrentState();
    handleInput();
}

void NavigationMenuController::render() const {
    if (_menuView) {
        _menuView->render(_currentMenuModel);
    }
}

IMenuView* NavigationMenuController::getMenuView() const {
    return _menuView.get();
}
