#include "BoardController.h"
#include "SettingsData.h"

BoardController::BoardController() : ThemeController() { syncFromSettings(); }

void BoardController::applyTheme() { SettingsData::getInstance().setStoneTheme(_theme); }

void BoardController::applyColor() { SettingsData::getInstance().setBoardColor(_colorIndex); }

void BoardController::syncFromSettings() {
	_theme = SettingsData::getInstance().getStoneTheme();
	_colorIndex = SettingsData::getInstance().getBoardColor();
}
