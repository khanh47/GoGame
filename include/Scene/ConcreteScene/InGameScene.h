#pragma once
#include "Scene.h"
#include <memory>
#include "InGameMenuController.h"
#include "SavedGameList.h"
#include "PassButton.h"
#include "TextBox.h"
#include "EndGameBox.h"

// Forward declaration
class GameController;
class Game;

class InGameScene : public Scene {
public:
    InGameScene(const std::string &gameMode);
    ~InGameScene() override = default;

    void init(void) override;
    void update(float deltaTime) override;
    void render(void) override;
    void handleInput(void) override;
    void cleanup(void) override;

    bool isActive(void) const override;
    std::string getName(void) const override;
    std::string getGameStateName(void) const override;

    void onEnter(void) override;
    void onExit(void) override;

    bool shouldTransition(void) const override;

    // popup control functions
    void openSavedGameListPopup();
    void closeSavedGameListPopup();

    void openGameDataInputPopup();
    void closeGameDataInputPopup();
    void closeGameDataInputPopupAndCreate();

		bool isPopup();

private:
    std::unique_ptr<InGameMenuController> menuController;
    std::unique_ptr<PassButton> _passButton;
		std::unique_ptr<TextBox> _textBox;
		std::unique_ptr<EndGameBox> _endGameBox;
		GameController* _gameController = nullptr;
		std::string _gameModeSelected;

private:
		void initializeMenuController();
		void initializePassButton();
};
