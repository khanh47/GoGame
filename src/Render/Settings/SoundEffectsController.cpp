#include "SoundEffectsController.h"
#include "AudioManager.h"
#include "SettingsData.h"

SoundEffectsController::SoundEffectsController(AudioManager* audioManager)
    : SoundController(audioManager) {
    syncFromAudioManager();
}

void SoundEffectsController::applyVolume() {
    if (_audioManager) {
        _audioManager->setSoundEffectsVolume(_volume);
        SettingsData::getInstance().setSoundEffectsVolume(_volume);
    }
}

void SoundEffectsController::applyMute() {
    if (_audioManager) {
        _audioManager->setSoundEffectsMuted(_muted);
        SettingsData::getInstance().setSoundEffectsMuted(_muted);
    }
}

void SoundEffectsController::syncFromAudioManager() {
    if (_audioManager) {
        _volume = _audioManager->getSoundEffectsVolume();
        _muted = _audioManager->isSoundEffectsMuted();
    }
}
