#include "AudioManager.h"
#include <algorithm>
#include "ResourceManager.h"

void AudioManager::setVolume(float volume) {
  _volume = std::clamp(volume, 0.0f, 100.0f);
	SetMusicVolume(_music, _volume);
}

float AudioManager::getVolume() const {
  return _volume;
}

void AudioManager::playMusic(const std::string& alias, bool loop) {
	if (_alias == alias) return;
	if (_alias != "") UnloadMusicStream(_music);
	_music = ResourceManager::getInstance().getMusic(alias);
	PlayMusicStream(_music);
	setVolume(_volume);
	_alias = alias;
}



