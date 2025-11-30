#pragma once
#include "raylib.h"
#include <string>

class AudioManager {
public:
  void playMusic(const std::string& alias, bool loop);
  void stopMusic();
  void setVolume(float volume);      // 0.0 → 100.0
  float getVolume() const;

private:
  Music _music;
  std::string _alias;
  float _volume = 1.0f;
};

