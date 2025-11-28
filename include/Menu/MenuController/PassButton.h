#pragma once
#include "MenuController.h"
#include <string>

// Forward Declaration
class GameController;

class PassButton : public MenuController {
public:
    PassButton(GameController* gameModel);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createPassButton();     // dynamic list

private:
    GameController* _gameController;
};
