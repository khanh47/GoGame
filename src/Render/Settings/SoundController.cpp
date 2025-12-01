#include "SoundController.h"
#include "AudioManager.h"
#include "ResourceManager.h"
#include "ResourceManager.h"
#include <algorithm>
#include <cmath>

SoundController::SoundController(AudioManager* audioManager)
    : _audioManager(audioManager) {
    syncFromAudioManager();
}

void SoundController::render(Vector2 position, Vector2 size) const {
    // Calculate positions
    float iconX = position.x + PADDING;
    float iconY = position.y + (size.y - ICON_SIZE) / 2.0f;
    
    float sliderX = iconX + ICON_SIZE + PADDING;
    float sliderY = position.y + (size.y - SLIDER_HEIGHT) / 2.0f;
    float sliderWidth = size.x - ICON_SIZE - PADDING * 4 - 50.0f;
    
    float textX = sliderX + sliderWidth + PADDING;
    float textY = position.y + (size.y - 20.0f) / 2.0f;

    drawSoundIcon({iconX, iconY});
    drawVolumeSlider({sliderX, sliderY}, sliderWidth);
    drawVolumeText({textX, textY});
}

void SoundController::handleInput(Vector2 position, Vector2 size) {
    Vector2 mousePos = GetMousePosition();
    
    // Calculate positions
    float iconX = position.x + PADDING;
    float iconY = position.y + (size.y - ICON_SIZE) / 2.0f;
    
    float sliderX = iconX + ICON_SIZE + PADDING;
    float sliderY = position.y + (size.y - SLIDER_HEIGHT) / 2.0f;
    float sliderWidth = size.x - ICON_SIZE - PADDING * 4 - 50.0f;

    // Handle mute icon click
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isIconClicked(mousePos, {iconX, iconY})) {
        toggleMute();
    }

    // Handle slider interaction
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isSliderClicked(mousePos, {sliderX, sliderY}, sliderWidth)) {
        _isDraggingSlider = true;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        _isDraggingSlider = false;
    }

    if (_isDraggingSlider && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        float newVolume = calculateVolumeFromSlider(mousePos, {sliderX, sliderY}, sliderWidth);
        setVolume(newVolume);
    }
}

void SoundController::setVolume(float volume) {
    _volume = std::clamp(volume, 0.0f, 100.0f);
    applyVolume();
}

float SoundController::getVolume() const {
    return _volume;
}

void SoundController::toggleMute() {
    _muted = !_muted;
    applyMute();
}

bool SoundController::isMuted() const {
    return _muted;
}

void SoundController::drawVolumeSlider(Vector2 sliderPos, float sliderWidth) const {
    // Draw slider track background
    DrawRectangle((int)sliderPos.x, (int)sliderPos.y, (int)sliderWidth, (int)SLIDER_HEIGHT, LIGHTGRAY);
    DrawRectangleLinesEx({sliderPos.x, sliderPos.y, sliderWidth, SLIDER_HEIGHT}, 1.0f, DARKGRAY);

    // Draw filled portion
    float filledWidth = (sliderWidth * _volume) / 100.0f;
    Color fillColor = _muted ? GRAY : Color{100, 149, 237, 255};  // Cornflower blue when active
    DrawRectangle((int)sliderPos.x, (int)sliderPos.y, (int)filledWidth, (int)SLIDER_HEIGHT, fillColor);

    // Draw knob
    float knobX = sliderPos.x + filledWidth;
    float knobY = sliderPos.y + SLIDER_HEIGHT / 2.0f;
    Color knobColor = _muted ? DARKGRAY : Color{70, 130, 180, 255};  // Steel blue when active
    DrawCircle((int)knobX, (int)knobY, SLIDER_KNOB_RADIUS, knobColor);
    DrawCircleLines((int)knobX, (int)knobY, SLIDER_KNOB_RADIUS, BLACK);
}

void SoundController::drawSoundIcon(Vector2 iconPos) const {
    const char* alias = nullptr;
    if (_muted) {
        alias = "no_volume";
    } else if (_volume <= 33.0f) {
        alias = "low_volume";
    } else {
        alias = "high_volume";
    }
    Texture2D &tex = ResourceManager::getInstance().getTexture2D(alias);
    float desiredSize = (float)ICON_SIZE;
    float scale = desiredSize / (float)tex.width;
    DrawTextureEx(tex, iconPos, 0.0f, scale, WHITE);
}

void SoundController::drawVolumeText(Vector2 textPos) const {
    Font font = ResourceManager::getInstance().getFont("Boldonse");
    char volumeStr[8];
    snprintf(volumeStr, sizeof(volumeStr), "%d%%", (int)_volume);
    DrawTextEx(font, volumeStr, textPos, 18.0f, 1.0f, BLACK);
}

bool SoundController::isSliderClicked(Vector2 mousePos, Vector2 sliderPos, float sliderWidth) const {
    Rectangle sliderRect = {
        sliderPos.x - SLIDER_KNOB_RADIUS,
        sliderPos.y - SLIDER_KNOB_RADIUS,
        sliderWidth + SLIDER_KNOB_RADIUS * 2,
        SLIDER_HEIGHT + SLIDER_KNOB_RADIUS * 2
    };
    return CheckCollisionPointRec(mousePos, sliderRect);
}

bool SoundController::isIconClicked(Vector2 mousePos, Vector2 iconPos) const {
    Rectangle iconRect = {iconPos.x, iconPos.y, ICON_SIZE, ICON_SIZE};
    return CheckCollisionPointRec(mousePos, iconRect);
}

float SoundController::calculateVolumeFromSlider(Vector2 mousePos, Vector2 sliderPos, float sliderWidth) const {
    float relativeX = mousePos.x - sliderPos.x;
    return std::clamp((relativeX / sliderWidth) * 100.0f, 0.0f, 100.0f);
}

void SoundController::applyVolume() {
    // Base implementation does nothing - override in derived classes
}

void SoundController::applyMute() {
    // Base implementation does nothing - override in derived classes
}

void SoundController::syncFromAudioManager() {
    // Base implementation does nothing - override in derived classes
}
