#pragma once
#include "Scene.h"
#include <memory>
#include "InGameMenuController.h"
#include "SavedGameListMenuController.h"

// Forward declaration
class GameModel;

class InGameScene : public Scene {
public:
    InGameScene(const std::string &gameMode);
    ~InGameScene() override = default;

    void setDependencies(GameModel* gameModel);

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

    // UI handlers
    void initializeSavedGameList(); // builds the menu and attaches view
    void initializeGameDataInput(); // initializes input buffer
    void updatePopups(float dt);
    void renderPopups() const;

private:
    std::unique_ptr<InGameMenuController> menuController;
    std::unique_ptr<SavedGameListMenuController> _savedListController;
		GameModel* _gameModel;
    std::string _gameModeSelected;
    std::string _gameDataInputBuffer;
    bool isSavedGameListPopup = false;
    bool isGameDataInputPopup = false;
private:
		void initializeMenuController();
};
