#pragma once
#include "MenuController.h"

// Forward Declaration
class SettingsScene;

class SettingsMenuController : public MenuController {
public:
    SettingsMenuController(SettingsScene* settingsScene);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createSettingsMenu();     // dynamic list

private:
    SettingsScene* _settingsScene;
};

