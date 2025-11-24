#pragma once
#include <memory>
#include <string>
#include <optional>
#include <filesystem>
#include <vector>

// Forward declaration
class HUD;
class Game;
class Board;

class GameSnapShot {
public:
    // --- Core Game State ---
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<int>> validPlayer1;
    std::vector<std::vector<int>> validPlayer2;
    int currentPlayer = 1;
    int scorePlayer1 = 0;
    int scorePlayer2 = 0;
    std::string gameMode;

    // --- Metadata ---
    std::string note;
    uint64_t timestamp = 0;

public:
    GameSnapShot() = default;

    // Binary serialization
    void serialize(std::ostream& out) const;
    bool deserialize(std::istream& in);
};

class GameModel {
private:
    std::shared_ptr<HUD> _hud;
    std::shared_ptr<Game> _game;
		std::optional<std::string> _gameDataSelected;

// Game Data Manager
		std::vector<GameSnapShot> _history;
		size_t _historyIndex = -1;
		std::filesystem::path _dataDir;

		void trimHistoryAfterIndex();

public:
    GameModel();
    void render();
    void handleInput();
    void update(float deltaTime);

		bool isGameOver();
		int finalScorePlayer1();
		int finalScorePlayer2();

// Game Data Manager
		// snapshot history
		const std::vector<GameSnapShot> getHistory() { return _history; }
		const GameSnapShot createSnapShot() const;
		const GameSnapShot* currentSnapShot() const;
		void applySnapShot(const GameSnapShot& snap);
		void pushState(const GameSnapShot& snap);
		void pushResetState(const GameSnapShot& snap);
		void resetGame();
		void passGame();
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
    bool createNewSaveFile(const std::string& filename);
		bool saveToFile(const GameSnapShot& snap, const std::string& filename);
		std::optional<GameSnapShot> loadFromFile(const std::string& filename);
		std::vector<std::string> getSavedGamesList() const;
		bool deleteSavedGame(const std::string& filename);

		std::optional<std::string> latestSavedGameFileName() const;
};

