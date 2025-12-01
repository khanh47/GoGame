#pragma once
#include "raylib.h"
#include <string>

// Forward declaration
class AudioManager;

class SoundController {
public:
    SoundController(AudioManager* audioManager);
    virtual ~SoundController() = default;

    virtual void render(Vector2 position, Vector2 size) const;
    virtual void handleInput(Vector2 position, Vector2 size);

    void setVolume(float volume);
    float getVolume() const;
    void toggleMute();
    bool isMuted() const;

protected:
    AudioManager* _audioManager;
    float _volume = 50.0f;
    bool _muted = false;
    bool _isDraggingSlider = false;

    // UI constants
    static constexpr float SLIDER_HEIGHT = 8.0f;
    static constexpr float SLIDER_KNOB_RADIUS = 10.0f;
    static constexpr float ICON_SIZE = 30.0f;
    static constexpr float PADDING = 15.0f;

    // Draw functions
    void drawVolumeSlider(Vector2 sliderPos, float sliderWidth) const;
    void drawSoundIcon(Vector2 iconPos) const;
    void drawVolumeText(Vector2 textPos) const;

    // Interaction functions
    bool isSliderClicked(Vector2 mousePos, Vector2 sliderPos, float sliderWidth) const;
    bool isIconClicked(Vector2 mousePos, Vector2 iconPos) const;
    float calculateVolumeFromSlider(Vector2 mousePos, Vector2 sliderPos, float sliderWidth) const;

    // Override these in derived classes to apply to AudioManager
    virtual void applyVolume();
    virtual void applyMute();
    virtual void syncFromAudioManager();
};
