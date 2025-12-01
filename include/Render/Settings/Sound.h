#pragma once
#include "raylib.h"
#include <memory>
#include <vector>

// Forward declarations
class AudioManager;
class ExpandableMenuItemView;
class SoundEffectsController;
class MusicController;

class SoundSettings {
public:
    SoundSettings(AudioManager* audioManager);
    ~SoundSettings();

    void handleInput();
    void update();
    void render() const;

    void setPosition(Vector2 position);
    void setSize(Vector2 size);
    float getTotalHeight() const;

private:
    AudioManager* _audioManager;
    Vector2 _position = {0, 0};
    Vector2 _size = {400, 300};

    std::vector<std::unique_ptr<ExpandableMenuItemView>> _menuItems;
    std::unique_ptr<SoundEffectsController> _soundEffectsController;
    std::unique_ptr<MusicController> _musicController;

    void initializeMenuItems();
    void updateMenuItemPositions();
};

