#pragma once
#include "raylib.h"
#include <string>

// Theme types for board and stone design
enum class ThemeType { Classic, Cartoon, Aesthetic };

// Singleton to store settings persistently
class SettingsData {
public:
	static SettingsData &getInstance();

	// Sound settings
	void setSoundEffectsVolume(float volume);
	float getSoundEffectsVolume() const;
	void setSoundEffectsMuted(bool muted);
	bool isSoundEffectsMuted() const;

	void setMusicVolume(float volume);
	float getMusicVolume() const;
	void setMusicMuted(bool muted);
	bool isMusicMuted() const;

	// Theme settings - Board
	void setBoardTheme(ThemeType theme);
	ThemeType getBoardTheme() const;
	void setBoardColor(int colorIndex);
	int getBoardColor() const;
	Color getBoardColorAsColor() const;

	// Theme settings - Stone
	void setStoneTheme(ThemeType theme);
	ThemeType getStoneTheme() const;
	void setStoneColor(int colorIndex);
	int getStoneColor() const;

    int getBoardSize() const { return _boardSize; }
    void setBoardSize(int size) { _boardSize = size; }
private:
	SettingsData();
	~SettingsData() = default;

	SettingsData(const SettingsData &) = delete;
	SettingsData &operator=(const SettingsData &) = delete;

	// Sound settings
	float _soundEffectsVolume = 50.0f;
	float _musicVolume = 50.0f;
	bool _soundEffectsMuted = false;
	bool _musicMuted = false;

	// Theme settings
	ThemeType _boardTheme = ThemeType::Classic;
	int _boardColor = 0;
	ThemeType _stoneTheme = ThemeType::Classic;
	int _stoneColor = 0;

    int _boardSize = 19;
};
