#include "SettingsData.h"
#include <algorithm>
#include "ThemeController.h"

SettingsData& SettingsData::getInstance() {
    static SettingsData instance;
    return instance;
}

SettingsData::SettingsData() : _boardColor(5) {} // default color: light brown

// Sound settings
void SettingsData::setSoundEffectsVolume(float volume) {
    _soundEffectsVolume = std::clamp(volume, 0.0f, 100.0f);
}

float SettingsData::getSoundEffectsVolume() const {
    return _soundEffectsVolume;
}

void SettingsData::setSoundEffectsMuted(bool muted) {
    _soundEffectsMuted = muted;
}

bool SettingsData::isSoundEffectsMuted() const {
    return _soundEffectsMuted;
}

void SettingsData::setMusicVolume(float volume) {
    _musicVolume = std::clamp(volume, 0.0f, 100.0f);
}

float SettingsData::getMusicVolume() const {
    return _musicVolume;
}

void SettingsData::setMusicMuted(bool muted) {
    _musicMuted = muted;
}

bool SettingsData::isMusicMuted() const {
    return _musicMuted;
}

// Theme settings - Board
void SettingsData::setBoardTheme(ThemeType theme) {
    _boardTheme = theme;
}

ThemeType SettingsData::getBoardTheme() const {
    return _boardTheme;
}

void SettingsData::setBoardColor(int colorIndex) {
    _boardColor = colorIndex;
}

int SettingsData::getBoardColor() const {
    return _boardColor;
}

Color SettingsData::getBoardColorAsColor() const {
    const auto& palette = ThemeController::getColorPalette();
    return palette[_boardColor];
}

// Theme settings - Stone
void SettingsData::setStoneTheme(ThemeType theme) {
    _stoneTheme = theme;
}

ThemeType SettingsData::getStoneTheme() const {
    return _stoneTheme;
}

void SettingsData::setStoneColor(int colorIndex) {
    _stoneColor = colorIndex;
}

int SettingsData::getStoneColor() const {
    return _stoneColor;
}
