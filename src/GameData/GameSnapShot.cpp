#include "GameSnapShot.h"
#include "Game.h"

#include <cctype>
#include <fstream>
#include <iostream>

const std::string DIR_NAME = "data";

template <typename T> static void writeValue(std::ostream &out, const T &value) {
	out.write(reinterpret_cast<const char *>(&value), sizeof(value));
}

template <typename T> static bool readValue(std::istream &in, T &value) {
	if (in.read(reinterpret_cast<char *>(&value), sizeof(value)).good())
		return true;
	else
		return false;
}

static bool readVector(std::istream &in, std::vector<MoveRecord> &list) {
	int sz = 0;
	if (!readValue(in, sz)) return false;

	list.resize(sz);

	// Read grid data
	for (auto &move : list) {
		if (!readValue(in, move.row))
			return false;
		if (!readValue(in, move.col))
			return false;
		if (!readValue(in, move.color))
			return false;
	}
	return true;
}

static void writeVector(std::ostream &out, const std::vector<MoveRecord> &list) {
	int sz = list.size();
	writeValue(out, sz);

	for (auto &move : list) {
		writeValue(out, move.row);
		writeValue(out, move.col);
		writeValue(out, move.color);
	}
}

bool GameSnapShot::deserialize(std::istream &in) {
	int version = 0;
	if (!readValue(in, version))
		return false;

	// Read numeric data
	if (!readValue(in, dep))
		return false;
	if (!readValue(in, moveIndex))
		return false;
	if (!readValue(in, boardSize))
		return false;
	if (!readVector(in, moveHistory))
		return false;

	return true;
}

void GameSnapShot::serialize(std::ostream &out) const {
	int version = 3; // bump version because we added new data
	writeValue(out, version);

	// Write numeric game info
	writeValue(out, dep);
	writeValue(out, moveIndex);
	writeValue(out, boardSize);
	writeVector(out, moveHistory);
}

bool writeSnapshots(const float &time, const GameSnapShot& snap, const std::string &filename) {
	std::ofstream out(filename, std::ios::binary);
	if (!out)
		return false;

	// Write the number of snapshots
	writeValue(out, time);
	snap.serialize(out);

	return true;
}

bool readSnapshots(float &time, GameSnapShot &snap, const std::string &filename) {
	std::ifstream in(filename, std::ios::binary);
	if (!in)
		return false;

	if (!readValue(in, time)) {
		return false;
	}
	if (!snap.deserialize(in))
		return false;

	return true;
}
