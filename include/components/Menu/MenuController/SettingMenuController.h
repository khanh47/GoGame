#pragma once
#include "MenuController.h"

// Forward declaration
class MenuView;
class ButtonMenuView;

class SettingMenuController : public MenuController {
public: 
    SettingMenuController(std::shared_ptr<MenuComponent> menuSystem);
    void setViewStrategy(std::unique_ptr<IMenuView> view);
    void updateNavigationMenuForCurrent();

    void handleInput();
    void update();
    void render() const;

    IMenuView* getMenuView() const;
private:
    int _selectedSettingIndex = -1;
    int _selectedPieceThemeIndex = -1;
    std::shared_ptr<MenuComponent> _workerMenu = nullptr;
    std::shared_ptr<ButtonMenuView> _workerMenuView = nullptr;
    void computeWorkerMenuView();
    void renderPreviewPieceTheme() const;
};