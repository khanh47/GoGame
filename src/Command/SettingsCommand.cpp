#include "SettingsCommand.h"
#include "AudioManager.h"
#include "SettingsMenuController.h"

// SetVolumeCommand
SetVolumeCommand::SetVolumeCommand(AudioManager *audioManager, VolumeType type, float volume)
		: _audioManager(audioManager), _type(type), _volume(volume) {}

void SetVolumeCommand::execute() {
	if (!_audioManager)
		return;

	if (_type == VolumeType::SoundEffects) {
		_audioManager->setSoundEffectsVolume(_volume);
		SettingsData::getInstance().setSoundEffectsVolume(_volume);
	} else {
		_audioManager->setMusicVolume(_volume);
		SettingsData::getInstance().setMusicVolume(_volume);
	}
}

std::unique_ptr<ICommand> SetVolumeCommand::clone() const {
	return std::make_unique<SetVolumeCommand>(_audioManager, _type, _volume);
}

// ToggleMuteCommand
ToggleMuteCommand::ToggleMuteCommand(AudioManager *audioManager, MuteType type)
		: _audioManager(audioManager), _type(type) {}

void ToggleMuteCommand::execute() {
	if (!_audioManager)
		return;

	if (_type == MuteType::SoundEffects) {
		bool newState = !_audioManager->isSoundEffectsMuted();
		_audioManager->setSoundEffectsMuted(newState);
		SettingsData::getInstance().setSoundEffectsMuted(newState);
	} else {
		bool newState = !_audioManager->isMusicMuted();
		_audioManager->setMusicMuted(newState);
		SettingsData::getInstance().setMusicMuted(newState);
	}
}

std::unique_ptr<ICommand> ToggleMuteCommand::clone() const {
	return std::make_unique<ToggleMuteCommand>(_audioManager, _type);
}

// SetThemeCommand
SetThemeCommand::SetThemeCommand(ThemeTarget target, ThemeType theme) : _target(target), _theme(theme) {}

void SetThemeCommand::execute() {
	if (_target == ThemeTarget::Board) {
		SettingsData::getInstance().setBoardTheme(_theme);
	} else {
		SettingsData::getInstance().setStoneTheme(_theme);
	}
}

std::unique_ptr<ICommand> SetThemeCommand::clone() const { return std::make_unique<SetThemeCommand>(_target, _theme); }

// SetColorCommand
SetColorCommand::SetColorCommand(ColorTarget target, int colorIndex) : _target(target), _colorIndex(colorIndex) {}

void SetColorCommand::execute() {
	if (_target == ColorTarget::Board) {
		SettingsData::getInstance().setBoardColor(_colorIndex);
	} else {
		SettingsData::getInstance().setStoneColor(_colorIndex);
	}
}

std::unique_ptr<ICommand> SetColorCommand::clone() const {
	return std::make_unique<SetColorCommand>(_target, _colorIndex);
}

// SwitchSettingsTabCommand
SwitchSettingsTabCommand::SwitchSettingsTabCommand(SettingsMenuController *controller, Tab tab)
		: _controller(controller), _tab(tab) {}

void SwitchSettingsTabCommand::execute() {
	if (_controller) {
		_controller->switchTab(_tab == Tab::Sound ? 0 : 1);
	}
}

std::unique_ptr<ICommand> SwitchSettingsTabCommand::clone() const {
	return std::make_unique<SwitchSettingsTabCommand>(_controller, _tab);
}

// Factory functions
std::unique_ptr<SetVolumeCommand> createSetVolumeCommand(AudioManager *audioManager, SetVolumeCommand::VolumeType type,
																												 float volume) {
	return std::make_unique<SetVolumeCommand>(audioManager, type, volume);
}

std::unique_ptr<ToggleMuteCommand> createToggleMuteCommand(AudioManager *audioManager,
																													 ToggleMuteCommand::MuteType type) {
	return std::make_unique<ToggleMuteCommand>(audioManager, type);
}

std::unique_ptr<SetThemeCommand> createSetThemeCommand(SetThemeCommand::ThemeTarget target, ThemeType theme) {
	return std::make_unique<SetThemeCommand>(target, theme);
}

std::unique_ptr<SetColorCommand> createSetColorCommand(SetColorCommand::ColorTarget target, int colorIndex) {
	return std::make_unique<SetColorCommand>(target, colorIndex);
}

std::unique_ptr<SwitchSettingsTabCommand> createSwitchSettingsTabCommand(SettingsMenuController *controller,
																																				 SwitchSettingsTabCommand::Tab tab) {
	return std::make_unique<SwitchSettingsTabCommand>(controller, tab);
}
