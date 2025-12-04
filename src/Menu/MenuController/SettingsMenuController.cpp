#include "SettingsMenuController.h"
#include "AudioManager.h"
#include "Menu.h"
#include "MenuCommand.h"
#include "MenuItem.h"
#include "MenuView.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SettingsScene.h"

#include <cmath>
#include <iostream>

SettingsMenuController::SettingsMenuController(SettingsScene *settingsScene, AudioManager *audioManager)
		: _settingsScene(settingsScene), _audioManager(audioManager), _currentTab(Tab::Sound) {
	_menuView = nullptr;

	// Initialize subscenes
	_soundSubscene = std::make_unique<SoundSettings>(audioManager);
	_themeSubscene = std::make_unique<Theme>();
    _gameSubscene = std::make_unique<GameSettings>();

	// Set positions and sizes for subscenes
	float screenWidth = (float)GetScreenWidth();
	float contentWidth = screenWidth - 100.0f;

	_soundSubscene->setPosition({50.0f, CONTENT_Y_OFFSET});
	_soundSubscene->setSize({contentWidth, 400.0f});

	_themeSubscene->setPosition({50.0f, CONTENT_Y_OFFSET});
	_themeSubscene->setSize({contentWidth, 400.0f});
	
    _gameSubscene->setPosition({50.0f, CONTENT_Y_OFFSET});
    _gameSubscene->setSize({contentWidth, 400.0f});
}

void SettingsMenuController::setViewStrategy(std::unique_ptr<IMenuView> view) {
	_menuView = std::move(view);
	if (_menuView && _menuSystem) {
		_menuView->createSettingsItemsViews(_menuSystem->getChildrens().size());
	}
}

void SettingsMenuController::handleInput() {
	handleTabNavigation();

	if (_currentTab == Tab::Sound && _soundSubscene) {
		_soundSubscene->handleInput();
	} else if (_currentTab == Tab::Theme && _themeSubscene) {
		_themeSubscene->handleInput();
	} else if (_currentTab == Tab::Game && _gameSubscene) {
        _gameSubscene->handleInput();
    }
}

void SettingsMenuController::update() {
	if (_currentTab == Tab::Sound && _soundSubscene) {
		_soundSubscene->update();
	} else if (_currentTab == Tab::Theme && _themeSubscene) {
		_themeSubscene->update();
	} else if (_currentTab == Tab::Game && _gameSubscene) {
        _gameSubscene->render();
    }
}

void SettingsMenuController::render() const {
	renderTabNavigation();

	if (_currentTab == Tab::Sound && _soundSubscene) {
		_soundSubscene->render();
	} else if (_currentTab == Tab::Theme && _themeSubscene) {
		_themeSubscene->render();
	} else if (_currentTab == Tab::Game && _gameSubscene) {
		_gameSubscene->render();
	}
}

void SettingsMenuController::createSettingsMenu() {
	_menuSystem = std::make_shared<Menu>("Settings selection", true);

	auto SoundItem = std::make_shared<MenuItem>("SOUND", true);
	_menuSystem->addItem(SoundItem);

	auto ThemeItem = std::make_shared<MenuItem>("THEME", true);
	_menuSystem->addItem(ThemeItem);

	auto GameItem = std::make_shared<MenuItem>("GAME", true);
	_menuSystem->addItem(GameItem);

	if (_menuView) {
		_menuView->createSettingsItemsViews(_menuSystem->getChildrens().size());
	}
}

void SettingsMenuController::switchTab(int tabIndex) {
	if (tabIndex == 0) {
		_currentTab = Tab::Sound;
	} else if (tabIndex == 1) {
		_currentTab = Tab::Theme;
	} else if (tabIndex == 2) {
		_currentTab = Tab::Game;
	}
}

void SettingsMenuController::renderTabNavigation() const {
	Font font = ResourceManager::getInstance().getFont("GozaruDemo");
	float screenWidth = (float)GetScreenWidth();

	// Calculate tab positions - centered
	float totalTabsWidth = 3 * TAB_WIDTH + 1 * TAB_SPACING;
	float startX = (screenWidth - totalTabsWidth) / 2.0f;
	float tabY = 10.0f;

	const char *tabNames[] = {"SOUND", "THEME", "GAME"};

	for (int i = 0; i < 3; i++) {
		float tabX = startX + i * (TAB_WIDTH + TAB_SPACING);

		bool isSelected = false;
		if (i == 0 && _currentTab == Tab::Sound)
			isSelected = true;
		if (i == 1 && _currentTab == Tab::Theme)
			isSelected = true;
		if (i == 2 && _currentTab == Tab::Game)
			isSelected = true;

		// Determine tab colors
		Color bgColor, textColor;
		if (isSelected) {
			bgColor = Color{100, 149, 237, 255};
			textColor = WHITE;
		} else {
			bgColor = Color{220, 220, 220, 255};
			textColor = BLACK;
		}

		// Check hover
		Vector2 mousePos = GetMousePosition();
		Rectangle tabRect = {tabX, tabY, TAB_WIDTH, TAB_HEIGHT};
		if (CheckCollisionPointRec(mousePos, tabRect) && !isSelected) {
			bgColor = Color{180, 180, 180, 255};
		}

		// Draw tab
		DrawRectangle((int)tabX, (int)tabY, (int)TAB_WIDTH, (int)TAB_HEIGHT, bgColor);
		DrawRectangleLinesEx(tabRect, 2.0f, DARKGRAY);

		// Draw text centered
		Vector2 textSize = MeasureTextEx(font, tabNames[i], 18.0f, 1.0f);
		float textX = tabX + (TAB_WIDTH - textSize.x) / 2.0f;
		float textY = tabY + (TAB_HEIGHT - textSize.y) / 2.0f;
		DrawTextEx(font, tabNames[i], {textX, textY}, 18.0f, 1.0f, textColor);
	}
}

void SettingsMenuController::handleTabNavigation() {
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		return;

	Vector2 mousePos = GetMousePosition();
	float screenWidth = (float)GetScreenWidth();

	// Calculate tab positions (2 tabs only: SOUND and THEME)
	float totalTabsWidth = 3 * TAB_WIDTH + 1 * TAB_SPACING;
	float startX = (screenWidth - totalTabsWidth) / 2.0f;
	float tabY = 10.0f;

	for (int i = 0; i < 3; i++) {
		float tabX = startX + i * (TAB_WIDTH + TAB_SPACING);
		Rectangle tabRect = {tabX, tabY, TAB_WIDTH, TAB_HEIGHT};

		if (CheckCollisionPointRec(mousePos, tabRect)) {
			if (i == 0) {
				_currentTab = Tab::Sound;
			} else if (i == 1) {
				_currentTab = Tab::Theme;
			} else if (i == 2) {
				_currentTab = Tab::Game;
			}
			break;
		}
	}
}
