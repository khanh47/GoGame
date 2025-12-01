#include "GameSnapShot.h"
#include <cctype>
#include <fstream>
#include <iostream>

const std::string DIR_NAME = "data";

// Helper lambdas for strings
static void writeString(std::ostream &out, const std::string &s) {
	uint64_t n = s.size();
	out.write(reinterpret_cast<const char *>(&n), sizeof(n));
	out.write(s.data(), (std::streamsize)n);
}

static bool readString(std::istream &in, std::string &s) {
	uint64_t n = 0;
	if (!in.read(reinterpret_cast<char *>(&n), sizeof(n)))
		return false;
	s.resize(n);
	if (n > 0 && !in.read(&s[0], (std::streamsize)n))
		return false;
	return true;
}

static void writeVector2D(std::ostream &out, const std::vector<std::vector<int>> &list) {
	uint64_t rows = list.size();
	uint64_t cols = (rows > 0 ? list[0].size() : 0);
	out.write(reinterpret_cast<const char *>(&rows), sizeof(rows));
	out.write(reinterpret_cast<const char *>(&cols), sizeof(cols));

	// Write list data
	for (const auto &row : list) {
		out.write(reinterpret_cast<const char *>(row.data()), sizeof(int) * cols);
	}
}

template <typename T> static void writeValue(std::ostream &out, const T &value) {
	out.write(reinterpret_cast<const char *>(&value), sizeof(value));
}

static bool readVector2D(std::istream &in, std::vector<std::vector<int>> &list) {
	uint64_t rows = 0, cols = 0;
	if (!in.read(reinterpret_cast<char *>(&rows), sizeof(rows)))
		return false;
	if (!in.read(reinterpret_cast<char *>(&cols), sizeof(cols)))
		return false;

	list.assign(rows, std::vector<int>(cols));

	// Read grid data
	for (auto &row : list) {
		if (!in.read(reinterpret_cast<char *>(row.data()), sizeof(int) * cols))
			return false;
	}
	return true;
}

template <typename T> static bool readValue(std::istream &in, T &value) {
	if (in.read(reinterpret_cast<char *>(&value), sizeof(value)).good())
		return true;
	else
		return false;
}

bool GameSnapShot::deserialize(std::istream &in) {
	uint32_t version = 0;
	if (!in.read(reinterpret_cast<char *>(&version), sizeof(version)))
		return false;

	if (!readVector2D(in, grid))
		return false;

	// NEW: read validPlayer1 + validPlayer2 (only if version >= 3)
	if (version >= 3) {
		if (!readVector2D(in, validPlayer1))
			return false;
		if (!readVector2D(in, validPlayer2))
			return false;
	} else {
		uint64_t rows = grid.size();
		uint64_t cols = (rows > 0 ? grid[0].size() : 0);

		validPlayer1.assign(rows, std::vector<int>(cols, 0));
		validPlayer2.assign(rows, std::vector<int>(cols, 0));
	}

	// Read numeric data
	if (!readValue(in, currentPlayer))
		return false;
	if (!readValue(in, scorePlayer1))
		return false;
	if (!readValue(in, scorePlayer2))
		return false;

	// Read strings
	if (!readString(in, gameMode))
		return false;
	if (!readString(in, note))
		return false;

	// Metadata
	if (!in.read(reinterpret_cast<char *>(&timestamp), sizeof(timestamp)))
		return false;

	return true;
}

void GameSnapShot::serialize(std::ostream &out) const {
	uint32_t version = 3; // bump version because we added new data
	out.write(reinterpret_cast<const char *>(&version), sizeof(version));

	writeVector2D(out, grid);
	writeVector2D(out, validPlayer1);
	writeVector2D(out, validPlayer2);

	// Write numeric game info
	writeValue(out, currentPlayer);
	writeValue(out, scorePlayer1);
	writeValue(out, scorePlayer2);

	// Write strings
	writeString(out, gameMode);
	writeString(out, note);

	// Metadata
	out.write(reinterpret_cast<const char *>(&timestamp), sizeof(timestamp));
}

bool writeSnapshots(const float &time, const int &index, const std::vector<GameSnapShot> &list,
										const std::string &filename) {
	std::ofstream out(filename, std::ios::binary);
	if (!out)
		return false;

	// Write the number of snapshots
	uint64_t count = list.size();
	writeValue(out, time);
	writeValue(out, index);
	writeValue(out, count);

	// Write each snapshot
	for (const auto &snap : list) {
		snap.serialize(out);
	}

	return true;
}

bool readSnapshots(float &time, int &index, std::vector<GameSnapShot> &list, const std::string &filename) {
	std::ifstream in(filename, std::ios::binary);
	if (!in)
		return false;

	if (!readValue(in, time)) {
		return false;
	}

	if (!readValue(in, index)) {
		return false;
	}
	uint64_t count = 0;
	if (!readValue(in, count)) {
		return false;
	}

	list.clear();
	list.reserve(count);

	for (uint64_t i = 0; i < count; i++) {
		GameSnapShot snap;
		if (!snap.deserialize(in))
			return false;
		list.push_back(std::move(snap));
	}

	return true;
}
