#pragma once
#include "MenuController.h"
#include "Sound.h"
#include "Theme.h"
#include <memory>

// Forward Declaration
class SettingsScene;
class AudioManager;

class SettingsMenuController : public MenuController {
public:
    enum class Tab {
        Sound = 0,
        Theme = 1
    };

    SettingsMenuController(SettingsScene* settingsScene, AudioManager* audioManager);

    void setViewStrategy(std::unique_ptr<IMenuView> view) override;
    void handleInput() override;
    void update() override;
    void render() const override;

    void createSettingsMenu();     // dynamic list
    void switchTab(int tabIndex);
    Tab getCurrentTab() const { return _currentTab; }

private:
    SettingsScene* _settingsScene;
    AudioManager* _audioManager;
    Tab _currentTab = Tab::Sound;

    std::unique_ptr<SoundSettings> _soundSubscene;
    std::unique_ptr<Theme> _themeSubscene;

    // Tab navigation UI
    void renderTabNavigation() const;
    void handleTabNavigation();

    // Tab UI constants
    static constexpr float TAB_HEIGHT = 40.0f;
    static constexpr float TAB_WIDTH = 100.0f;
    static constexpr float TAB_SPACING = 10.0f;
    static constexpr float CONTENT_Y_OFFSET = 60.0f;
};

