#include "raylib.h"
#include "SettingMenuController.h"
#include "ButtonMenuView.h"
#include "MenuComponent.h"
#include "MenuItemView.h"
#include "ICommand.h"
#include "ResourceManager.h"
#include <memory>
#include <vector>
#include <iostream>

SettingMenuController::SettingMenuController(std::shared_ptr<MenuComponent> menuSystem)
{
    _menuSystem = menuSystem;
    _currentMenuModel = menuSystem;
    setViewStrategy(std::make_unique<ButtonMenuView>());
}

void SettingMenuController::setViewStrategy(std::unique_ptr<IMenuView> view)
{
    _menuView = std::move(view);
    if (_menuView)
    {
        int numberOfItems = static_cast<int>(_currentMenuModel->getChildrens().size());
        _menuView->createSettingMenuItemViews(numberOfItems);
    }
}

void SettingMenuController::updateNavigationMenuForCurrent()
{
    // Setting menu is static, no need to update
}

void SettingMenuController::handleInput()
{
    Vector2 mousePosition = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (!_menuView)
    {
        std::cout << "Menu view is not set!" << std::endl;
        return;
    }

    const std::vector<std::shared_ptr<MenuItemView>> &itemViews = _menuView->getItemViews();
    const auto &menuItems = _currentMenuModel->getChildrens();

    for (size_t i = 0; i < itemViews.size() && i < menuItems.size(); ++i)
    {
        bool hovered = itemViews[i]->isPointed(mousePosition);
        itemViews[i]->setHovered(hovered);

        if (hovered && mouseClicked)
        {
            if (i != _selectedSettingIndex)
            {
                _selectedSettingIndex = static_cast<int>(i);
            }
        }
    }

    if (_workerMenu)
    {
        const std::vector<std::shared_ptr<MenuItemView>> &workerItemViews = _workerMenuView->getItemViews();
        const auto &workerMenuItems = _workerMenu->getChildrens();

        for (size_t i = 0; i < workerItemViews.size() && i < workerMenuItems.size(); ++i)
        {
            bool hovered = workerItemViews[i]->isPointed(mousePosition);
            workerItemViews[i]->setHovered(hovered);

            if (hovered && mouseClicked)
            {
                _selectedPieceThemeIndex = static_cast<int>(i);
                auto command = workerMenuItems[i]->cloneCommand();
                if (command)
                {
                    command->execute();
                }
            }
        }
    }
}

void SettingMenuController::computeWorkerMenuView()
{
    if (!_workerMenu)
        return;

    int numberOfItems = _workerMenu->getChildrens().size();
    if (!_workerMenuView)
    {
        _workerMenuView = std::make_shared<ButtonMenuView>();
    }

    std::vector<std::shared_ptr<MenuItemView>> itemViews;
    const float verticalSpacing = 20.0f; // spacing between items
    const float itemHeight = 40.0f;
    const float itemWidth = 200;
    const float menuWidth = (_selectedSettingIndex == 0) ? 300.0f : float(GetScreenWidth());
    const float menuY = (_selectedSettingIndex == 0) ? 0.0f : 100.0f;
    const Rectangle menuArea = {0, menuY, menuWidth, (float)GetScreenHeight()}; // Example menu area

    const float startX = menuArea.x + (menuArea.width - itemWidth) / 2;
    const float startY = menuArea.y + (menuArea.height - (numberOfItems * itemHeight + (numberOfItems - 1) * verticalSpacing)) / 2;

    itemViews.reserve(numberOfItems); // Reserve space for active items
    for (int i = 0; i < numberOfItems; ++i)
    {
        Vector2 position = {startX, startY + i * (itemHeight + verticalSpacing)};
        Vector2 size = {itemWidth, itemHeight};
        auto itemView = std::make_shared<MenuItemView>(position, size);
        itemView->setFont(ResourceManager::getInstance().getFont("public_sans_bold"));
        itemViews.push_back(itemView);
    }
    _workerMenuView->setItemViews(itemViews);
}

void SettingMenuController::update()
{
    if (_selectedSettingIndex >= 0 && _selectedSettingIndex < (int)(_menuSystem->getChildrens().size())) {
        if (_workerMenu != _menuSystem->getChildrens()[_selectedSettingIndex]) {
            _workerMenu = _menuSystem->getChildrens()[_selectedSettingIndex];
            std::cout << "Selected setting changed to: " << _workerMenu->getTitle() << std::endl;
            computeWorkerMenuView();            
        }
    }


    auto ItemViews = _menuView->getItemViews();
    for (auto itemView : ItemViews) {
        if (itemView) {
            itemView->setSelected(false);
        }
    }
    int selectedIndex = _selectedSettingIndex;
    if (selectedIndex >= 0 && selectedIndex < (int)(ItemViews.size())) {
        ItemViews[selectedIndex]->setSelected(true);
    }

    if (_workerMenu) {
        auto workerItemViews = _workerMenuView->getItemViews();
        for (auto itemView : workerItemViews) {
            if (itemView) {
                itemView->setSelected(false);
            }
        }
        int selectedPieceIndex = _selectedPieceThemeIndex;
        if (selectedPieceIndex >= 0 && selectedPieceIndex < (int)(workerItemViews.size())) {
            workerItemViews[selectedPieceIndex]->setSelected(true);
        }
    }
}

void SettingMenuController::render() const
{
    if (_menuView)
    {
        _menuView->render(_currentMenuModel);
    }

    if (_workerMenuView && _workerMenu)
    {
        _workerMenuView->render(_workerMenu);
    }
    if (_workerMenu && _selectedSettingIndex >= 0)
    {
        renderPreviewPieceTheme();
    }
}

void SettingMenuController::renderPreviewPieceTheme() const
{
    // Implementation for rendering the preview of the selected piece theme
}