#pragma once
#include <vector>
#include <stack>
#include <unordered_set>

const int MAX = 400;

inline int encodePos(int r, int c, int cols) { return r * cols + c; }
inline std::pair<int, int> decodePos(int code, int cols) { return {code / cols, code % cols}; }

struct Node {
	bool valid;
	int color;
	std::unordered_set<int> liberties;
	std::unordered_set<int> positions;

	Node() {
		valid = false;
		color = 0;
	}
};

struct GroupChange {
	int root;
	int lab;
	Node node;

	GroupChange(int _root = 0, int _lab = -1, Node _node = Node()) {
		root = _root; lab = _lab; node = _node;
	}
};

struct LibertyChange {
	int root;
	int value;
	bool wasInsert;
};

class GroupManager {
public:
	GroupManager(int rows, int cols);
	~GroupManager() = default;

	void reset();
	int find(int u);
	bool unite(int u, int v);

	void pushGroupChange(GroupChange info);
	GroupChange getGroupChange(int u) { return GroupChange(u, _lab[u], _groups[u]); }
	void pushLibertyChange(int root, int liberty, bool isInsert);

	int getValue(int row, int col);
	int getGroupSize(int row, int col);
	int getGroupLiberties(int row, int col);
	bool isRoot(int row, int col);

	void createNewGroup(int row, int col, int color);
	void resetGroup(int root);
	void mergeGroups(int keep, int merge);
	void removeGroup(int root, std::vector<std::pair<int, int>>& removedStones);
	std::pair<bool, std::vector<std::pair<int, int>>> makeMove(int row, int col, int color);

	int getTerritory(int color);
	std::vector<std::pair<int, int>> getValidMoves(int radius, int color);
	bool applyMove(int row, int col, int color);
	bool isSelfCaptured(int row, int col, int color);
	void rollBack(int groupChangeCount, int libertyChangeCount);

	void undo();

	bool isOutside(int row, int col) { return row < 0 || row >= _rows || col < 0 || col >= _cols; }
	int encode(int row, int col) { return encodePos(row, col, _cols); }
	std::pair<int, int> decode(int pos) { return decodePos(pos, _cols); }

private:
	int _rows;
	int _cols;

	int _groupChangeCount = 0;
	int _libertyChangeCount = 0;
	std::vector<int> _lab;
	std::vector<Node> _groups;

	std::stack<GroupChange> _groupChanges;
	std::stack<LibertyChange> _libertyChanges;
	std::stack<std::pair<int, int>> _history;

  static constexpr int dx[4] = {1, 0, -1, 0};
  static constexpr int dy[4] = {0, 1, 0, -1};
};

