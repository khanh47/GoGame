#include "GameModel.h"
#include "HUD.h"
#include "Game.h"
#include "json.hpp"
#include <fstream>
#include <chrono>
#include <iostream>
#include <cctype>

// Helper lambdas for strings
static void writeString(std::ostream& out, const std::string& s) {
    uint64_t n = s.size();
    out.write(reinterpret_cast<const char*>(&n), sizeof(n));
    out.write(s.data(), (std::streamsize)n);
}

static bool readString(std::istream& in, std::string& s) {
    uint64_t n = 0;
    if (!in.read(reinterpret_cast<char*>(&n), sizeof(n))) return false;
    s.resize(n);
    if (n > 0 && !in.read(&s[0], (std::streamsize)n)) return false;
    return true;
}

void GameSnapShot::serialize(std::ostream& out) const {
    uint32_t version = 2;   // versioning for future changes
    out.write(reinterpret_cast<const char*>(&version), sizeof(version));

    // Write grid size
    uint64_t rows = grid.size();
    uint64_t cols = (rows > 0 ? grid[0].size() : 0);

    out.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
    out.write(reinterpret_cast<const char*>(&cols), sizeof(cols));

    // Write grid data
    for (const auto& row : grid) {
        out.write(reinterpret_cast<const char*>(row.data()), sizeof(int) * cols);
    }

    // Write numeric game info
    out.write(reinterpret_cast<const char*>(&currentPlayer), sizeof(currentPlayer));
    out.write(reinterpret_cast<const char*>(&scorePlayer1), sizeof(scorePlayer1));
    out.write(reinterpret_cast<const char*>(&scorePlayer2), sizeof(scorePlayer2));

    // Write strings
    writeString(out, gameMode);
    writeString(out, note);

    // Metadata
    out.write(reinterpret_cast<const char*>(&timestamp), sizeof(timestamp));
}

bool GameSnapShot::deserialize(std::istream& in) {
    uint32_t version = 0;
    if (!in.read(reinterpret_cast<char*>(&version), sizeof(version)))
        return false;

    // Read grid dimensions
    uint64_t rows = 0, cols = 0;
    if (!in.read(reinterpret_cast<char*>(&rows), sizeof(rows))) return false;
    if (!in.read(reinterpret_cast<char*>(&cols), sizeof(cols))) return false;

    grid.assign(rows, std::vector<int>(cols));

    // Read grid data
    for (auto& row : grid) {
        if (!in.read(reinterpret_cast<char*>(row.data()), sizeof(int) * cols))
            return false;
    }

    // Read numeric data
    if (!in.read(reinterpret_cast<char*>(&currentPlayer), sizeof(currentPlayer))) return false;
    if (!in.read(reinterpret_cast<char*>(&scorePlayer1), sizeof(scorePlayer1))) return false;
    if (!in.read(reinterpret_cast<char*>(&scorePlayer2), sizeof(scorePlayer2))) return false;

    // Read strings
    if (!readString(in, gameMode)) return false;
    if (!readString(in, note)) return false;

    // Metadata
    if (!in.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp))) return false;

    return true;
}

GameModel::GameModel()
{
		_dataDir = std::filesystem::current_path() / "data";
		if (!std::filesystem::exists(_dataDir)) {
			std::error_code ec;
			std::filesystem::create_directories(_dataDir, ec);
					if (ec) {
							std::cerr << "Cannot create Data directory: " << ec.message() << std::endl;
					}
		}
		_history.clear();
		_historyIndex = -1;
    _hud = std::make_shared<HUD>();
    _game = std::make_shared<Game>(19, 19); // Standard Go board size
		pushState(createSnapShot());
}

void GameModel::render() {
    if (!_game || !_hud) return;
    _game->render();
    _hud->render();
}

void GameModel::handleInput() {
    if (!_game || !_hud) return;
    if (_game->handleInput()) {
				pushState(createSnapShot());
		}
}

void GameModel::update() {
    if (!_game || !_hud) return;
		const GameSnapShot* snap = currentSnapShot();
		applySnapShot(*snap);
}

void GameModel::trimHistoryAfterIndex() {
		if (_historyIndex + 1 < _history.size()) {
				_history.erase(_history.begin() + _historyIndex + 1, _history.end());
		}
}

const GameSnapShot GameModel::createSnapShot() const {
		GameSnapShot snap;
		snap.grid = _game->getGrid();
		snap.currentPlayer = _game->getCurrentPlayer();
		snap.scorePlayer1 = _game->getScorePlayer1();
		snap.scorePlayer2 = _game->getScorePlayer2();
		snap.gameMode = _game->getGameMode();
		return snap;
}

const GameSnapShot* GameModel::currentSnapShot() const {
		if (_history.empty()) return nullptr;
		return &_history[_historyIndex];
}

void GameModel::applySnapShot(const GameSnapShot& snap) {
		int currentPlayer = snap.currentPlayer;
		int scorePlayer1 = snap.scorePlayer1;
		int scorePlayer2 = snap.scorePlayer2;
		_game->loadFromSnapShot(snap.grid, currentPlayer, scorePlayer1, scorePlayer2);
		_hud->getScores(scorePlayer1, scorePlayer2, currentPlayer);
}

void GameModel::pushState(const GameSnapShot& snap) {
		trimHistoryAfterIndex();
		_history.push_back(snap);
		_historyIndex = _history.size() - 1;
}

void GameModel::resetGame() {
    _game = std::make_shared<Game>(19, 19); // Standard Go board size
		pushState(createSnapShot());
}

bool GameModel::canUndo() const {
		return !_history.empty() && _historyIndex > 0;
}

bool GameModel::canRedo() const {
		return !_history.empty() && _historyIndex + 1 < _history.size();
}

bool GameModel::undo() {
		if (!canUndo()) return false;
		_historyIndex--;
		return true;
}

bool GameModel::redo() {
		if (!canRedo()) return false;
		_historyIndex++;
		return true;
}

bool GameModel::saveToFile(const GameSnapShot& snap, const std::string& filename) {
		std::filesystem::path p = _dataDir / filename;
		std::ofstream out(p, std::ios::binary);
		if (!out) return false;
		snap.serialize(out);
		return out.good();
}

std::optional<GameSnapShot> GameModel::loadFromFile(const std::string& filename) {
		std::filesystem::path p = _dataDir / filename;
		if (!std::filesystem::exists(p)) return std::nullopt;
		std::ifstream in(p, std::ios::binary);
		if (!in) return std::nullopt;
		GameSnapShot snap;
		if (!snap.deserialize(in)) return std::nullopt;
		return snap;
}

std::vector<std::string> GameModel::getSavedGamesList() const {
		std::vector<std::string> out;
		if (!std::filesystem::exists(_dataDir)) return out;
		for (auto & entry : std::filesystem::directory_iterator(_dataDir)) {
				if (entry.is_regular_file()) {
						out.push_back(entry.path().filename().string());
				}
		}
		std::sort(out.begin(), out.end());
		return out;
}

bool GameModel::deleteSavedGame(const std::string& filename) {
		std::filesystem::path p = _dataDir / filename;
		std::error_code ec;
		return std::filesystem::remove(p, ec);
}

std::optional<std::string> GameModel::latestSavedGameFileName() const {
		if (!std::filesystem::exists(_dataDir)) return std::nullopt;
		std::string best;
		std::filesystem::file_time_type bestTime;
		bool first = true;
		for (auto &entry : std::filesystem::directory_iterator(_dataDir)) {
				if (!entry.is_regular_file()) continue;
				auto ft = entry.last_write_time();
				if (first || ft > bestTime) {
					bestTime = ft;
					best = entry.path().filename().string();
					first = false;
				}
		}
		if (first) return std::nullopt;
		return best;
}

bool GameModel::saveCurrentToSelectedFile() {
    if (!_gameDataSelected.has_value()) return false;
    const std::string &fname = _gameDataSelected.value();
    // make sure we have a current snapshot
    if (_history.empty() && !(_game)) return false;
    GameSnapShot snap = createSnapShot();
    return saveToFile(snap, fname);
}

bool GameModel::deleteSelectedFile() {
    if (!_gameDataSelected.has_value()) return false;
    bool ok = deleteSavedGame(_gameDataSelected.value());
    if (ok) {
        // clear selection if removed
        _gameDataSelected.reset();
    }
    return ok;
}

bool GameModel::createNewSaveFile(const std::string& filename) {
    if (filename.empty()) return false;

    // basic validation: must start with alnum or '_' or '-' and not contain path separators
    char c = filename[0];
    if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-' || c == '.')) return false;
    if (filename.find_first_of("/\\") != std::string::npos) return false;

    // optionally append extension, e.g., .save; skip here to keep names as-is
    GameSnapShot snap = createSnapShot();
    bool ok = saveToFile(snap, filename);
    if (ok) {
        // refresh selection to new file
        _gameDataSelected = filename;
    }
    return ok;
}
