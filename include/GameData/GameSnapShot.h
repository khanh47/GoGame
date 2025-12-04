#pragma once
#include <string>
#include <vector>
#include "Game.h"

class GameSnapShot {
public:
	int dep;
	int moveIndex;
	int boardSize;
	std::vector<MoveRecord> moveHistory;

public:
	GameSnapShot() = default;

	// Binary serialization
	void serialize(std::ostream &out) const;
	bool deserialize(std::istream &in);
};

bool writeSnapshots(const float &time, const GameSnapShot &snap, const std::string &filename);
bool readSnapshots(float &time, GameSnapShot &snap, const std::string &filename);
