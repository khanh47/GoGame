#pragma once
#include "raylib.h"
#include <string>

class AudioManager {
public:
	void playMusic(const std::string &alias);
	void stopMusic();
	void setVolume(float volume); // 0.0 → 100.0
	float getVolume() const;
	void update();

	// Sound effects volume controls
	void setSoundEffectsVolume(float volume); // 0.0 → 100.0
	float getSoundEffectsVolume() const;
	void setSoundEffectsMuted(bool muted);
	bool isSoundEffectsMuted() const;

	// Music volume controls
	void setMusicVolume(float volume); // 0.0 → 100.0
	float getMusicVolume() const;
	void setMusicMuted(bool muted);
	bool isMusicMuted() const;

	// Play sound effect with current volume settings
	void playSoundEffect(const std::string &alias);

private:
	Music *_music = nullptr;
	std::string _alias;
	float _volume = 50.0f;
	bool _musicLoaded = false;

	// Separate volume controls
	float _soundEffectsVolume = 50.0f;
	float _musicVolume = 50.0f;
	bool _soundEffectsMuted = false;
	bool _musicMuted = false;

	void updateMusicVolume();
};
