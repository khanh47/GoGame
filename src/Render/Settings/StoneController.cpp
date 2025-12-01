#include "StoneController.h"
#include "SettingsData.h"

StoneController::StoneController() : ThemeController() { syncFromSettings(); }

void StoneController::applyTheme() { SettingsData::getInstance().setStoneTheme(_theme); }

void StoneController::applyColor() { SettingsData::getInstance().setStoneColor(_colorIndex); }

void StoneController::syncFromSettings() {
	_theme = SettingsData::getInstance().getStoneTheme();
	_colorIndex = SettingsData::getInstance().getStoneColor();
}
