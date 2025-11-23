#include "InGameScene.h"
#include "GameModel.h"
#include "ResourceManager.h"
#include "ButtonMenuView.h"
#include "ListMenuView.h"
#include "raylib.h"
#include <iostream>

InGameScene::InGameScene(const std::string &gameMode)
    : _gameModeSelected(gameMode) {
		init();
}

void InGameScene::setDependencies(GameModel* gameModel) {
}

void InGameScene::init(void) {
		if (_gameModeSelected.empty()) return;
		_gameModel = new GameModel();
		if (_gameModeSelected != "PVP" && _gameModeSelected != "PVE") {
		    auto savedFiles = _gameModel->getSavedGamesList();
				std::cout << "Game data: " << _gameModeSelected << std::endl;

		    for (auto& file : savedFiles) {
		        if (file == _gameModeSelected) {
		            auto snapOpt = _gameModel->loadFromFile(file);

		            if (snapOpt.has_value()) {
		                _gameModel->applySnapShot(snapOpt.value());
		            }

		            break;
		        }
		    }
		}
		initializeMenuController();
		initializeSavedGameList();
		initializeGameDataInput();
		std::cout << "InGameScene initialize" << std::endl;
}

void InGameScene::update(float deltaTime) {
    updatePopups(deltaTime);
    if (isSavedGameListPopup || isGameDataInputPopup) {
        return;
    }

    if (menuController)
        menuController->update();

    if (_gameModel)
        _gameModel->update();
}

void InGameScene::render(void) {
    Texture2D background = ResourceManager::getInstance().getTexture2D("in_game_background");
    DrawTextureEx(background, { 0, 0 }, 0.0f, 1.3f, WHITE);
    if (_gameModel) {
        _gameModel->render();
    }
		if (menuController) {
				menuController->render();
		}
		renderPopups();
}

void InGameScene::handleInput() {
		if (isGameDataInputPopup) {
				return;
		}
		if (isSavedGameListPopup) {
				_savedListController->handleInput();
		}

    if (menuController)
        menuController->handleInput();
    if (_gameModel)
        _gameModel->handleInput();
}

void InGameScene::cleanup(void) {
}

bool InGameScene::isActive(void) const {
    return _isActive;
}

std::string InGameScene::getName(void) const {
    return "InGameScene";
}

std::string InGameScene::getGameStateName(void) const {
    return "IN_GAME";
}

void InGameScene::onEnter(void) {
    _isActive = true;
}

void InGameScene::onExit(void) {
    _isActive = false;
}

bool InGameScene::shouldTransition(void) const {
    return false;
}

void InGameScene::initializeMenuController() {
    menuController = std::make_unique<InGameMenuController>(this, _gameModel);

    menuController->setViewStrategy(std::make_unique<ButtonMenuView>());
    menuController->createInGameMenu();
}

void InGameScene::openSavedGameListPopup() {
		isSavedGameListPopup = true;
}

void InGameScene::closeSavedGameListPopup() {
    isSavedGameListPopup = false;
}

void InGameScene::openGameDataInputPopup() {
    isGameDataInputPopup = true;
    _gameDataInputBuffer.clear();
}

void InGameScene::closeGameDataInputPopup() {
    isGameDataInputPopup = false;
    _gameDataInputBuffer.clear();
}

void InGameScene::initializeSavedGameList() {
    _savedListController = std::make_unique<SavedGameListMenuController>(this, _gameModel);

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

    _savedListController->setViewStrategy(std::move(listMenuView));
    _savedListController->createSavedGameListMenu();
}

void InGameScene::initializeGameDataInput() {
    _gameDataInputBuffer.clear();
}

void InGameScene::updatePopups(float dt) {
    if (isSavedGameListPopup) {
        if (_savedListController) _savedListController->update();
        return;
    }

    if (isGameDataInputPopup) {
        // capture text input
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125) _gameDataInputBuffer.push_back(static_cast<char>(key));
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !_gameDataInputBuffer.empty()) {
            _gameDataInputBuffer.pop_back();
        }
        if (IsKeyPressed(KEY_ENTER)) {
            // Validate the buffer and create file
            if (!_gameDataInputBuffer.empty()) {
                // simple validation: first char must be alnum or '_' or '-' or '.'
                char fc = _gameDataInputBuffer.front();
                if (std::isalnum(static_cast<unsigned char>(fc)) || fc=='_' || fc=='-' || fc=='.') {
                    bool ok = _gameModel->createNewSaveFile(_gameDataInputBuffer);
                    if (ok) {
                        // refresh saved-list if open
                        if (_savedListController) {
                            _savedListController->createSavedGameListMenu();
                        }
                        closeGameDataInputPopup();
                        return;
                    }
                }
            }
            // if invalid or creation fails, keep typing (or show a message)
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            closeGameDataInputPopup();
            return;
        }

        return; // block further update
    }
}

void InGameScene::renderPopups() const {
    if (!isSavedGameListPopup && !isGameDataInputPopup) return;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

    if (isSavedGameListPopup) {
        const int x = 220, y = 100, w = GetScreenWidth() - 440, h = GetScreenHeight() - 200;
        DrawRectangle(x, y, w, h, Color{240,230,200,255});
        DrawRectangleLines(x, y, w, h, DARKGRAY);

        if (_savedListController) {
            _savedListController->render();
        }
    }

    if (isGameDataInputPopup) {
        int bx = GetScreenWidth()/2 - 300;
        int by = GetScreenHeight()/2 - 60;
        DrawRectangle(bx, by, 600, 140, WHITE);
        DrawRectangleLines(bx, by, 600, 140, DARKGRAY);
        DrawText("Enter save name:", bx + 12, by + 8, 22, BLACK);

        std::string display = _gameDataInputBuffer.empty() ? "<type here>" : _gameDataInputBuffer;
        DrawText(display.c_str(), bx + 12, by + 48, 24, BLACK);

        DrawText("Enter = create, Esc = cancel", bx + 12, by + 96, 18, DARKGRAY);
    }
}

