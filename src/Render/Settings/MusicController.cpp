#include "MusicController.h"
#include "AudioManager.h"
#include "SettingsData.h"

MusicController::MusicController(AudioManager* audioManager)
    : SoundController(audioManager) {
    syncFromAudioManager();
}

void MusicController::applyVolume() {
    if (_audioManager) {
        _audioManager->setMusicVolume(_volume);
        SettingsData::getInstance().setMusicVolume(_volume);
    }
}

void MusicController::applyMute() {
    if (_audioManager) {
        _audioManager->setMusicMuted(_muted);
        SettingsData::getInstance().setMusicMuted(_muted);
    }
}

void MusicController::syncFromAudioManager() {
    if (_audioManager) {
        _volume = _audioManager->getMusicVolume();
        _muted = _audioManager->isMusicMuted();
    }
}
