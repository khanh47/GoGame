#pragma once
#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include "GameSnapShot.h"

// Forward declaration
class Game;
class Board;

class DataManager {
private:
	Game* _game;
	Board* _board;
	std::optional<std::string> _gameDataSelected;

// Game Data Manager
	std::vector<GameSnapShot> _history;
	size_t _historyIndex = -1;

	void trimHistoryAfterIndex();

public:
  DataManager(Game* game, Board* board);
  void handleInput();
  void update();

		// snapshot history
	const std::vector<GameSnapShot> getHistory();
	const GameSnapShot createSnapShot() const;
	const GameSnapShot* currentSnapShot() const;
	void applySnapShot(const GameSnapShot& snap);
	void addState();
	void pushState(const GameSnapShot& snap);
	bool canUndo() const;
	bool canRedo() const;
	bool undo();
	bool redo();

		// save/load snapshot
  void setSelectedGameData(const std::optional<std::string>& name) { _gameDataSelected = name; }
	std::optional<std::string> getSelectedGameData() const { return _gameDataSelected; }
	bool isGameDataSelected() { return _gameDataSelected.has_value(); }

  bool saveCurrentToSelectedFile();
  bool deleteSelectedFile();
	bool deleteSavedGame(const std::string& filename);

	bool loadFromFile(const std::string& filename);

	static std::vector<std::string> getSavedGamesList();
	static std::optional<std::string> latestSavedGameFileName();
	static std::filesystem::path _dataDir;
};

bool createNewSaveFile(const std::string& filename);
