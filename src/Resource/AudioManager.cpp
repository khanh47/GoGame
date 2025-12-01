#include "Resource/AudioManager.h"
#include "Resource/ResourceManager.h"
#include <algorithm>
#include <iostream>

static float clamp01_from100(float v) {
  return std::clamp(v / 100.0f, 0.0f, 1.0f);
}

void AudioManager::setVolume(float volume) {
  // legacy alias to music volume
  setMusicVolume(volume);
  _volume = std::clamp(volume, 0.0f, 100.0f);
}

float AudioManager::getVolume() const {
  return _volume;
}

void AudioManager::update() {
  // Must be called every frame while a music stream is playing
  if (_music && _musicLoaded) {
    UpdateMusicStream(*_music);
  }
}

void AudioManager::playMusic(const std::string& alias) {
  // If already playing this alias, do nothing (but ensure volume applied)
  if (_music && _alias == alias && _musicLoaded) {
    updateMusicVolume();
    return;
  }

  // If currently playing different music, stop it (do NOT unload)
  if (_music && _musicLoaded) {
    StopMusicStream(*_music);
    _musicLoaded = false;
  }

  // Get the music reference from ResourceManager (ResourceManager owns the Music lifetime)
  try {
    Music& mref = ResourceManager::getInstance().getMusic(alias);
    _music = &mref;
    _alias = alias;
    PlayMusicStream(*_music);
    _musicLoaded = true;
    updateMusicVolume();
    std::cout << "Play song: " << alias << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "AudioManager::playMusic error: " << e.what() << std::endl;
    _music = nullptr;
    _alias.clear();
    _musicLoaded = false;
  }
}

void AudioManager::stopMusic() {
  if (_music && _musicLoaded) {
    StopMusicStream(*_music);
    _musicLoaded = false;
  }
}

void AudioManager::setSoundEffectsVolume(float volume) {
  _soundEffectsVolume = std::clamp(volume, 0.0f, 100.0f);
}

float AudioManager::getSoundEffectsVolume() const {
  return _soundEffectsVolume;
}

void AudioManager::setSoundEffectsMuted(bool muted) {
  _soundEffectsMuted = muted;
}

bool AudioManager::isSoundEffectsMuted() const {
  return _soundEffectsMuted;
}

void AudioManager::setMusicVolume(float volume) {
  _musicVolume = std::clamp(volume, 0.0f, 100.0f);
  updateMusicVolume();
}

float AudioManager::getMusicVolume() const {
  return _musicVolume;
}

void AudioManager::setMusicMuted(bool muted) {
  _musicMuted = muted;
  updateMusicVolume();
}

bool AudioManager::isMusicMuted() const {
  return _musicMuted;
}

void AudioManager::updateMusicVolume() {
  if (!_music || !_musicLoaded) return;
  float effectiveVolume = _musicMuted ? 0.0f : clamp01_from100(_musicVolume);
  SetMusicVolume(*_music, effectiveVolume);
}

void AudioManager::playSoundEffect(const std::string& alias) {
  if (_soundEffectsMuted) return;
  try {
    Sound& sref = ResourceManager::getInstance().getSound(alias);
    float effectiveVolume = clamp01_from100(_soundEffectsVolume);
    SetSoundVolume(sref, effectiveVolume);
    PlaySound(sref);
  } catch (const std::exception& e) {
    std::cerr << "AudioManager::playSoundEffect error: " << e.what() << std::endl;
  }
}