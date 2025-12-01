#pragma once
#include <cstdint>
#include <string>
#include <vector>

// Forward declaration
class Game;

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
	void serialize(std::ostream &out) const;
	bool deserialize(std::istream &in);
};

bool writeSnapshots(const float &time, const int &index, const std::vector<GameSnapShot> &list,
										const std::string &filename);
bool readSnapshots(float &time, int &index, std::vector<GameSnapShot> &list, const std::string &filename);
