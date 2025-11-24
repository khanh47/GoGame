#pragma once
#include "MenuController.h"
#include <string>

// Forward Declaration
class GameModel;

class PassButton : public MenuController {
public:
    PassButton(GameModel* gameModel);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createPassButton();     // dynamic list

private:
    GameModel* _gameModel;
};
