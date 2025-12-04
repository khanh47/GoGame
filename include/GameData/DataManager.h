#pragma once
#include "GameSnapShot.h"
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

// Forward declaration
class Game;
class Board;

class DataManager {
private:
	Game *_game;
	std::optional<std::string> _gameDataSelected;
	int _menuVersion = 0;
	float _timeCount = 0.0f;

public:
	DataManager(Game *game);
	void update(float deltaTime);
	float getTime() { return _timeCount; }
	int getMenuVersion() const { return _menuVersion; }

	// snapshot history
	const std::vector<GameSnapShot> getHistory();
	const GameSnapShot createSnapShot() const;
	void applySnapShot(const GameSnapShot &snap);

	// save/load snapshot
	void setSelectedGameData(const std::optional<std::string> &name);
	std::optional<std::string> getSelectedGameData() const { return _gameDataSelected; }
	std::string getSelectedGameDataName() const { return _gameDataSelected.value(); }
	bool isGameDataSelected() { return _gameDataSelected.has_value(); }

	bool saveCurrentToSelectedFile();
	bool deleteSelectedFile();
	bool deleteSavedGame(const std::string &filename);
	bool createNewSaveFile(const std::string &filename);

	bool loadFromFile(const std::string &filename);

	static std::vector<std::string> getSavedGamesList();
	static std::optional<std::string> latestSavedGameFileName();
	static std::filesystem::path _dataDir;
};
