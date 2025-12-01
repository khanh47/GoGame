#pragma once
#include "MenuController.h"
#include <string>

// Forward Declaration
class InGameScene;

class TextBoxMenuController : public MenuController {
public:
    TextBoxMenuController(InGameScene* inGameScene);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createMenu();     // dynamic list

private:
    InGameScene* _inGameScene;
};
