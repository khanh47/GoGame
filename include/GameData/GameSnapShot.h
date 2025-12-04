#pragma once
#include <string>
#include <vector>
#include "Game.h"

class GameSnapShot {
public:
	int rows = 0;
	int cols = 0;
	bool isAIEnabled = false;
	int dep = 0;
	int moveIndex;
	std::vector<MoveRecord> moveHistory;

public:
	GameSnapShot() = default;

	// Binary serialization
	void serialize(std::ostream &out) const;
	bool deserialize(std::istream &in);
};

bool writeSnapshots(const float &time, const GameSnapShot &snap, const std::string &filename);
bool readSnapshots(float &time, GameSnapShot &snap, const std::string &filename);
