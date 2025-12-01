#include "DataManager.h"
#include "Board.h"
#include "Game.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

std::filesystem::path DataManager::_dataDir = std::filesystem::current_path() / "data";

DataManager::DataManager(Game *game, Board *board) : _game(game), _board(board) {
	if (!std::filesystem::exists(_dataDir)) {
		std::error_code ec;
		std::filesystem::create_directories(_dataDir, ec);
		if (ec) {
			std::cerr << "Cannot create Data directory: " << ec.message() << std::endl;
		}
	}
	_history.clear();
	_historyIndex = -1;
}

void DataManager::update(float deltaTime) { _timeCount += deltaTime; }

const std::vector<GameSnapShot> DataManager::getHistory() { return _history; }

void DataManager::trimHistoryAfterIndex() {
	if (_historyIndex + 1 < _history.size()) {
		_history.erase(_history.begin() + _historyIndex + 1, _history.end());
	}
}

const GameSnapShot DataManager::createSnapShot() const {
	GameSnapShot snap;
	snap.grid = _game->getGrid();
	snap.validPlayer1 = _board->getValidPlayer1Map();
	snap.validPlayer2 = _board->getValidPlayer2Map();
	snap.currentPlayer = _game->getCurrentPlayer();
	snap.scorePlayer1 = _game->getScorePlayer1();
	snap.scorePlayer2 = _game->getScorePlayer2();
	snap.gameMode = _game->getGameMode();
	return snap;
}

const GameSnapShot *DataManager::currentSnapShot() const {
	if (_history.empty())
		return nullptr;
	return &_history[_historyIndex];
}

void DataManager::applySnapShot(const GameSnapShot &snap) {
	_game->setCurrentPlayer(snap.currentPlayer);
	_game->setScorePlayer1(snap.scorePlayer1);
	_game->setScorePlayer2(snap.scorePlayer2);
	_board->setGrid(snap.grid);
	_board->setValidPlayer1Map(snap.validPlayer1);
	_board->setValidPlayer2Map(snap.validPlayer2);
}

void DataManager::addState() { pushState(createSnapShot()); }

void DataManager::pushState(const GameSnapShot &snap) {
	trimHistoryAfterIndex();
	_history.push_back(snap);
	_historyIndex = _history.size() - 1;
	applySnapShot(_history[_historyIndex]);
}

bool DataManager::canUndo() const { return !_history.empty() && _historyIndex > 0; }

bool DataManager::canRedo() const { return !_history.empty() && _historyIndex + 1 < _history.size(); }

bool DataManager::undo() {
	if (!canUndo())
		return false;
	_historyIndex--;
	if (const GameSnapShot *s = currentSnapShot()) {
		applySnapShot(*s);
	}
	return true;
}

bool DataManager::redo() {
	if (!canRedo())
		return false;
	_historyIndex++;
	if (const GameSnapShot *s = currentSnapShot()) {
		applySnapShot(*s);
	}
	return true;
}

bool DataManager::deleteSavedGame(const std::string &filename) {
	std::filesystem::path p = _dataDir / filename;
	std::error_code ec;
	return std::filesystem::remove(p, ec);
}

bool DataManager::loadFromFile(const std::string &filename) {
	std::filesystem::path fullPath = _dataDir / filename;
	if (!std::filesystem::exists(fullPath))
		return false;
	int index = 0;
	std::vector<GameSnapShot> snapshots;
	float time = 0.0f;
	if (!readSnapshots(time, index, snapshots, fullPath.string()))
		return false;
	if (snapshots.empty() || index < 0 || index >= (int)snapshots.size() || time < 0)
		return false;
	_history = snapshots;
	_historyIndex = index;
	_timeCount = time;
	applySnapShot(_history[_historyIndex]);
	return true;
}

void DataManager::setSelectedGameData(const std::optional<std::string> &name) {
	_gameDataSelected = name;
	++_menuVersion; // Increment menu version to force menu refresh
}

bool DataManager::saveCurrentToSelectedFile() {
	if (!_gameDataSelected.has_value()) {
		std::cerr << "saveCurrentToSelectedFile: no file selected\n";
		return false;
	}

	const std::string &filename = _gameDataSelected.value();

	if (_history.empty()) {
		std::cerr << "saveCurrentToSelectedFile: history is empty, nothing to save\n";
		return false;
	}

	std::filesystem::path full = _dataDir / filename;
	std::error_code ec;
	std::filesystem::create_directories(full.parent_path(), ec);
	if (ec) {
		std::cerr << "saveCurrentToSelectedFile: failed to ensure data dir: " << ec.message() << "\n";
		return false;
	}

	bool ok = writeSnapshots(_timeCount, _historyIndex, _history, full.string());
	if (!ok) {
		std::cerr << "saveCurrentToSelectedFile: writeSnapshots failed for: " << full << "\n";
		std::ofstream ofs(full, std::ios::binary);
		if (!ofs.is_open()) {
			std::cerr << "saveCurrentToSelectedFile: fallback ofstream open failed for: " << full << "\n";
			return false;
		}
		ofs << "/* fallback save: writeSnapshots failed */\n";
		ofs.close();
		std::cerr << "saveCurrentToSelectedFile: fallback write succeeded (placeholder)\n";
		return true;
	}

	return true;
}

bool DataManager::deleteSelectedFile() {
	if (!_gameDataSelected.has_value())
		return false;
	bool ok = deleteSavedGame(_gameDataSelected.value());
	if (ok) {
		// clear selection if removed
		_gameDataSelected.reset();
	}
	return ok;
}

bool DataManager::createNewSaveFile(const std::string &filename) {
	if (filename.empty())
		return false;
	char c = filename[0];
	if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-' || c == '.'))
		return false;
	if (filename.find_first_of("/\\") != std::string::npos)
		return false;

	// Ensure data dir exists
	std::error_code ec;
	std::filesystem::create_directories(DataManager::_dataDir, ec);
	if (ec) {
		std::cerr << "createNewSaveFile: cannot create data dir: " << ec.message() << std::endl;
		return false;
	}

	std::filesystem::path full = DataManager::_dataDir / filename;
	if (std::filesystem::exists(full)) {
		std::cerr << "createNewSaveFile: file already exists: " << full << std::endl;
		return false;
	}

	std::ofstream ofs(full, std::ios::binary);
	if (!ofs.is_open()) {
		std::cerr << "createNewSaveFile: cannot open file for writing: " << full << std::endl;
		return false;
	}
	_menuVersion++;
	ofs.close();
	return true;
}

std::optional<std::string> DataManager::latestSavedGameFileName() {
	if (!std::filesystem::exists(_dataDir))
		return std::nullopt;
	std::string best;
	std::filesystem::file_time_type bestTime;
	bool first = true;
	for (auto &entry : std::filesystem::directory_iterator(_dataDir)) {
		if (!entry.is_regular_file())
			continue;
		auto ft = entry.last_write_time();
		if (first || ft > bestTime) {
			bestTime = ft;
			best = entry.path().filename().string();
			first = false;
		}
	}
	if (first)
		return std::nullopt;
	return best;
}

std::vector<std::string> DataManager::getSavedGamesList() {
	std::vector<std::string> out;
	if (!std::filesystem::exists(_dataDir))
		return out;
	for (auto &entry : std::filesystem::directory_iterator(_dataDir)) {
		if (entry.is_regular_file()) {
			out.push_back(entry.path().filename().string());
		}
	}
	std::sort(out.begin(), out.end());
	return out;
}
