#pragma once
#include "ICommand.h"
#include "SettingsData.h"
#include <functional>

// Forward declarations
class AudioManager;
class SettingsMenuController;

// Command for changing volume
class SetVolumeCommand : public ICommand {
public:
    enum class VolumeType {
        SoundEffects,
        Music
    };

    SetVolumeCommand(AudioManager* audioManager, VolumeType type, float volume);

    void execute() override;
    void undo() override {}
    void redo() override {}
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }

    std::string getName() const override { return "Set Volume Command"; }
    std::unique_ptr<ICommand> clone() const override;
    CommandType getType() const override { return CommandType::NON_STATE; }

private:
    AudioManager* _audioManager;
    VolumeType _type;
    float _volume;
};

// Command for toggling mute
class ToggleMuteCommand : public ICommand {
public:
    enum class MuteType {
        SoundEffects,
        Music
    };

    ToggleMuteCommand(AudioManager* audioManager, MuteType type);

    void execute() override;
    void undo() override {}
    void redo() override {}
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }

    std::string getName() const override { return "Toggle Mute Command"; }
    std::unique_ptr<ICommand> clone() const override;
    CommandType getType() const override { return CommandType::NON_STATE; }

private:
    AudioManager* _audioManager;
    MuteType _type;
};

// Command for setting theme
class SetThemeCommand : public ICommand {
public:
    enum class ThemeTarget {
        Board,
        Stone
    };

    SetThemeCommand(ThemeTarget target, ThemeType theme);

    void execute() override;
    void undo() override {}
    void redo() override {}
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }

    std::string getName() const override { return "Set Theme Command"; }
    std::unique_ptr<ICommand> clone() const override;
    CommandType getType() const override { return CommandType::NON_STATE; }

private:
    ThemeTarget _target;
    ThemeType _theme;
};

// Command for setting color
class SetColorCommand : public ICommand {
public:
    enum class ColorTarget {
        Board,
        Stone
    };

    SetColorCommand(ColorTarget target, int colorIndex);

    void execute() override;
    void undo() override {}
    void redo() override {}
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }

    std::string getName() const override { return "Set Color Command"; }
    std::unique_ptr<ICommand> clone() const override;
    CommandType getType() const override { return CommandType::NON_STATE; }

private:
    ColorTarget _target;
    int _colorIndex;
};

// Command for switching settings tabs
class SwitchSettingsTabCommand : public ICommand {
public:
    enum class Tab {
        Sound,
        Theme
    };

    SwitchSettingsTabCommand(SettingsMenuController* controller, Tab tab);

    void execute() override;
    void undo() override {}
    void redo() override {}
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }

    std::string getName() const override { return "Switch Settings Tab Command"; }
    std::unique_ptr<ICommand> clone() const override;
    CommandType getType() const override { return CommandType::IMMEDIATE; }

private:
    SettingsMenuController* _controller;
    Tab _tab;
};

// Factory functions
std::unique_ptr<SetVolumeCommand> createSetVolumeCommand(AudioManager* audioManager, 
    SetVolumeCommand::VolumeType type, float volume);
std::unique_ptr<ToggleMuteCommand> createToggleMuteCommand(AudioManager* audioManager, 
    ToggleMuteCommand::MuteType type);
std::unique_ptr<SetThemeCommand> createSetThemeCommand(SetThemeCommand::ThemeTarget target, 
    ThemeType theme);
std::unique_ptr<SetColorCommand> createSetColorCommand(SetColorCommand::ColorTarget target, 
    int colorIndex);
std::unique_ptr<SwitchSettingsTabCommand> createSwitchSettingsTabCommand(
    SettingsMenuController* controller, SwitchSettingsTabCommand::Tab tab);
