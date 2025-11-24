#include "SavedGameList.h"
#include "ListMenuView.h"
#include "ButtonMenuView.h"
#include "GameModel.h"
#include "InGameScene.h"
#include "colors.h"
#include "raylib.h"

SavedGameList::SavedGameList(InGameScene* scene, GameModel* model)
    : _inGameScene(scene), _gameModel(model) {}

void SavedGameList::open() {
    if (_open) return;
    _open = true;
		initializeSavedGameListMenuController();
		initializeSavedGameMenuController();
}

void SavedGameList::close() {
    _open = false;
    _listMenuController.reset();
		_menuController.reset();
}

void SavedGameList::initializeSavedGameListMenuController() {
    _listMenuController = std::make_unique<SavedGameListMenuController>(_inGameScene, _gameModel);

    // Create the ListMenuView with custom positioning
    Rectangle menuArea = {
        (float)(GetScreenWidth() / 2 - 150),  // Center horizontally
        150,                                   // Start below the title
        300,                                   // Width
        400                                    // Height
    };

    auto listMenuView = std::make_unique<ListMenuView>(menuArea);
    listMenuView->setItemHeight(50.0f);
    listMenuView->setItemSpacing(10.0f);
    listMenuView->setScrollbarWidth(20.0f);

    _listMenuController->setViewStrategy(std::move(listMenuView));
    _listMenuController->createSavedGameListMenu();
}

void SavedGameList::initializeSavedGameMenuController() {
    _menuController = std::make_unique<SavedGameMenuController>(_inGameScene, _gameModel);

    _menuController->setViewStrategy(std::make_unique<ButtonMenuView>());
    _menuController->createSavedGameMenu();
}

void SavedGameList::update(float dt) {
    if (!_open || !_listMenuController || !_menuController) return;
    _listMenuController->update();
    _menuController->update();
}

void SavedGameList::render() const {
    if (!_open || !_listMenuController || !_menuController) return;

		int screenWidth = GetScreenWidth();
		int screenHeight = GetScreenHeight();
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.6f));
		Rectangle box = { (screenWidth - 800) / 2.0f, (screenHeight - 600) / 2.0f, 800, 600 };
		DrawRectangleRounded(box, 0.2f, 16, overlay);

    _listMenuController->render();
		_menuController->render();
}

void SavedGameList::handleInput() {
    if (!_open || !_listMenuController || !_menuController) return;
    _listMenuController->handleInput();
		if (_gameModel->isGameDataSelected())
				initializeSavedGameMenuController();
    _menuController->handleInput();
}
