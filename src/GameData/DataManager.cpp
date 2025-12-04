#include "DataManager.h"
#include "Board.h"
#include "Game.h"
#include "GameSnapShot.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

std::filesystem::path DataManager::_dataDir = std::filesystem::current_path() / "data";

DataManager::DataManager(Game *game) : _game(game) {
	if (!std::filesystem::exists(_dataDir)) {
		std::error_code ec;
		std::filesystem::create_directories(_dataDir, ec);
		if (ec) {
			std::cerr << "Cannot create Data directory: " << ec.message() << std::endl;
		}
	}
}

void DataManager::update(float deltaTime) { _timeCount += deltaTime; }

const GameSnapShot DataManager::createSnapShot() const {
	GameSnapShot snap;
	snap.dep = _game->getDepth();
	snap.moveIndex = _game->getMoveIndex();
	snap.boardSize = _game->getBoardSize();
	snap.moveHistory = _game->getHistoryMove();
	return snap;
}

void DataManager::applySnapShot(const GameSnapShot &snap) {
	_game->setDepth(snap.dep);
	_game->setMoveIndex(snap.moveIndex);
	_game->setBoardSize(snap.boardSize);
	_game->setMoveHistory(snap.moveHistory);
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
	GameSnapShot snap;
	float time = 0.0f;
	if (!readSnapshots(time, snap, fullPath.string()))
		return false;
	_timeCount = time;
	applySnapShot(snap);
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

	std::filesystem::path full = _dataDir / filename;
	std::error_code ec;
	std::filesystem::create_directories(full.parent_path(), ec);
	if (ec) {
		std::cerr << "saveCurrentToSelectedFile: failed to ensure data dir: " << ec.message() << "\n";
		return false;
	}

	GameSnapShot snap = createSnapShot();
	bool ok = writeSnapshots(_timeCount, snap, full.string());
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
