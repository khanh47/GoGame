#include "GroupManager.h"

#include <queue>
#include <iostream>

constexpr int GroupManager::dx[4];
constexpr int GroupManager::dy[4];

GroupManager::GroupManager(int rows, int cols) : _rows(rows), _cols(cols) {
	reset();
}

void GroupManager::reset() {
  _lab.assign(_rows * _cols, -1);
	_groups.assign(_rows * _cols, Node());
	_groupChangeCount = 0;
	_libertyChangeCount = 0;
	while (_groupChanges.size()) _groupChanges.pop();
	while (_libertyChanges.size()) _libertyChanges.pop();
	while (_history.size()) _history.pop();
	_history.push({0, 0});
}

int GroupManager::find(int u) {
	return _lab[u] < 0 ? u : find(_lab[u]);
}

bool GroupManager::unite(int u, int v) {
	u = find(u);
	v = find(v);
	if (u == v) return false;
	if (_lab[u] > _lab[v]) std::swap(u, v);

	pushGroupChange(getGroupChange(u));
	pushGroupChange(getGroupChange(v));

	_lab[u] += _lab[v];
	_lab[v] = u;
	mergeGroups(u, v);
	return true;
}

void GroupManager::pushGroupChange(GroupChange info) {
	_groupChangeCount++;
	_groupChanges.push(info);
}

void GroupManager::pushLibertyChange(int root, int liberty, bool isInsert) {
	_libertyChangeCount++;
	_libertyChanges.push({root, liberty, isInsert});
}

int GroupManager::getValue(int row, int col) {
	int pos = encode(row, col);
	return _groups[find(pos)].color;
}

int GroupManager::getGroupSize(int row, int col) {
	int pos = encode(row, col);
	return -_lab[find(pos)];
}

int GroupManager::getGroupLiberties(int row, int col) {
	int pos = encode(row, col);
	return _groups[find(pos)].liberties.size();
}

bool GroupManager::isRoot(int row, int col) {
	int pos = encode(row, col);
	return pos == find(pos);
}

void GroupManager::createNewGroup(int row, int col, int color) {
	const int pos = encode(row, col);
	if (_groups[pos].valid) return;
	pushGroupChange(getGroupChange(pos));

	_lab[pos] = -1;
	_groups[pos].valid = true;
	_groups[pos].color = color;

	for (int i = 0; i < 4; i++) {
		int nrow = row + dx[i],
				ncol = col + dy[i];
		if (isOutside(nrow, ncol)) continue;
		int npos = encode(nrow, ncol);
		npos = find(npos);
		if (_groups[npos].valid) {
			pushLibertyChange(npos, pos, 0);
			_groups[npos].liberties.erase(pos);
		} else {
			_groups[pos].liberties.insert(npos);
		}
	}

	_groups[pos].positions.insert(pos);
}

void GroupManager::resetGroup(int root) {
	_groups[root].liberties.clear();
	_groups[root].positions.clear();
	_groups[root].color = 0;
	_groups[root].valid = false;
}

void GroupManager::mergeGroups(int keep, int merge) {
	for (int i : _groups[merge].positions) {
		_groups[keep].positions.insert(i);
	}
	for (int i : _groups[merge].liberties) {
		_groups[keep].liberties.insert(i);
	}
	for (int i : _groups[keep].positions) {
		_groups[keep].liberties.erase(i);
	}
	resetGroup(merge);
}

void GroupManager::removeGroup(int root, std::vector<std::pair<int, int>>& removedStones) {
	for (int pos : _groups[root].positions) {
		pushGroupChange(getGroupChange(pos));

		_lab[pos] = -1;
		auto [row, col] = decodePos(pos, _cols);
		removedStones.push_back({row, col});

		for (int i = 0; i < 4; i++) {
			int nrow = row + dx[i],
					ncol = col + dy[i];
			if (isOutside(nrow, ncol)) continue;
			int npos = encode(nrow, ncol);
			npos = find(npos);
			if (_groups[npos].valid && _groups[npos].color != _groups[root].color) {
				pushLibertyChange(npos, pos, 1);
				_groups[npos].liberties.insert(pos);
			}
		}
	}

	_lab[root] = -1;
	resetGroup(root);
}

std::pair<bool, std::vector<std::pair<int, int>>> GroupManager::makeMove(int row, int col, int color) {
	_history.push({_groupChangeCount, _libertyChangeCount});

	createNewGroup(row, col, color);
	int pos = encode(row, col);

	// Merge current stone to 4 adjacent stones
	for (int i = 0; i < 4; i++) {
		int nrow = row + dx[i],
				ncol = col + dy[i];
		if (isOutside(nrow, ncol)) continue;
		int npos = encode(nrow, ncol);
		npos = find(npos);
		if (_groups[npos].valid && color == _groups[npos].color) {
			unite(pos, npos);
		}
	}

	// Check to capture adjacent opponent stones
	std::vector<std::pair<int, int>> capturedStones;
	for (int i = 0; i < 4; i++) {
		int nrow = row + dx[i],
				ncol = col + dy[i];
		if (isOutside(nrow, ncol)) continue;
		int npos = encode(nrow, ncol);
		npos = find(npos);
		if (_groups[npos].valid && color != _groups[npos].color) {
			if (_groups[npos].liberties.empty()) {
				removeGroup(npos, capturedStones);
			}
		}
	}

	// Check if self captured
	pos = find(pos);
	if (_groups[pos].liberties.empty()) {
		std::vector<std::pair<int, int>> removedStones;
		removeGroup(pos, removedStones);
		return {false, {}};
	}

	return {true, capturedStones};
}

bool GroupManager::isSelfCaptured(int row, int col, int color) {
	if (isOutside(row, col) || getValue(row, col)) return false;

	for (int i = 0; i < 4; i++) {
		int nrow = row + dx[i],
				ncol = col + dy[i];
		if (isOutside(nrow, ncol)) continue;
		int npos = encode(nrow, ncol);
		npos = find(npos);
		if (!_groups[npos].valid) {
			return false;
		}
		if (color == _groups[npos].color) {
			if (_groups[npos].liberties.size() > 1) {
				return false;
			}
		} else {
			if (_groups[npos].liberties.size() == 1) {
				return false;
			}
		}
	}

	return true;
}

bool GroupManager::isSeki(int row, int col) {
	if (isOutside(row, col) || getValue(row, col)) return false;
	int blackGroupCount = 0;
	int whiteGroupCount = 0;
	int lastBlackGroup = -1;
	int lastWhiteGroup = -1;
	bool seki = true;

	for (int i = 0; i < 4; i++) {
		int nrow = row + dx[i],
				ncol = col + dy[i];
		if (isOutside(nrow, ncol)) continue;
		int npos = encode(nrow, ncol);
		npos = find(npos);
		if (!_groups[npos].valid) continue;
		if (_groups[npos].liberties.size() != 2) {
			return false;
		}
		if (_groups[npos].color == 1) {
			if (lastBlackGroup != npos) {
				blackGroupCount++;
				lastBlackGroup = npos;
			}
		} else {
			if (lastWhiteGroup != npos) {
				whiteGroupCount++;
				lastWhiteGroup = npos;
			}
		}
	}

	return blackGroupCount == 1 && whiteGroupCount == 1;
}

std::pair<int, int> GroupManager::getTerritory(int &myScore, int &oppScore, int color) {
	std::vector<bool> visited(_rows * _cols, false);
	std::vector<int> eyeCount(_rows * _cols, 0);
	int myEye = 0;
	int oppEye = 0;

	for (int row = 0; row < _rows; row++) {
		for (int col = 0; col < _cols; col++) {
			int pos = encode(row, col);
			pos = find(pos);

			if (_groups[pos].valid || visited[pos]) continue;

			std::queue<int> q;
			std::vector<int> groupList;
			int count = 0;
			bool touchedColor = false;
			bool touchedOther = false;

			visited[pos] = true;
			q.push(pos);

			while (q.size()) {
				int pos = q.front(); q.pop();
				auto [row, col] = decode(pos);
				count++;
				for (int i = 0; i < 4; i++) {
					int nrow = row + dx[i],
							ncol = col + dy[i];
					if (isOutside(nrow, ncol)) continue;
					int npos = encode(nrow, ncol);
					npos = find(npos);
					if (_groups[npos].valid) {
						groupList.push_back(pos);
						if (_groups[npos].color == color) {
							touchedColor = true;
						} else {
							touchedOther = true;
						}
					} else if (!visited[npos]) {
						visited[npos] = true;
						q.push(npos);
					}
				}
			}
			if (touchedColor && !touchedOther) {
				for (int i : groupList) {
					eyeCount[i]++;
					if (eyeCount[i] == 2) {
						myEye++;
					}
				}
				myScore += count;
			}
			if (!touchedColor && touchedOther) {
				oppScore += count;
				for (int i : groupList) {
					eyeCount[i]++;
					if (eyeCount[i] == 2) {
						oppEye++;
					}
				}
			}
		}
	}
	return {myEye, oppEye};
}

std::vector<std::pair<int, int>> GroupManager::getValidMoves(int radius, int color) {
	bool hasStone = false;
	for (int row = 0; row < _rows && !hasStone; row++) {
		for (int col = 0; col < _cols && !hasStone; col++) {
			if (getValue(row, col)){
				hasStone = true;
			}
		}
	}
	if (!hasStone) {
		return {{_rows >> 1, _cols >> 1}};
	}

	std::vector<std::pair<int, int>> validMoves;
	std::vector<bool> visited(_rows * _cols, 0);
	std::vector<int> owner(_rows * _cols, 0);
	std::vector<int> groupSize(_rows * _cols, 0);
	std::vector<int> groupID(_rows * _cols, 0);

	const int GR_SIZE_LIM = 8;
	int oppColor = 3 - color;
	int groupCount = 0;

	for (int row = 0; row < _rows; row++) {
		for (int col = 0; col < _cols; col++) {
			int pos = encode(row, col);
			pos = find(pos);
			if (getValue(row, col) || isSelfCaptured(row, col, color) || isSeki(row, col)) continue;
			if (visited[pos] && owner[groupID[pos]] == oppColor) continue;
			bool canGet = false;
			for (int dx = -radius; dx <= radius && !canGet; dx++) {
				for (int dy = -radius; dy <= radius && std::abs(dy) + std::abs(dx) <= radius; dy++) {
					int nrow = row + dx,
							ncol = col + dy;
					if (isOutside(nrow, ncol)) continue;
					int npos = encode(nrow, ncol);
					npos = find(npos);
					if (_groups[npos].valid) {
						canGet = true;
						break;
					}
				}
			}
			if (!canGet) continue;
			if (!visited[pos]) {
				std::queue<int> q;
				int count = 0;
				bool touchedColor = false;
				bool touchedOther = false;

				groupCount++;
				visited[pos] = true;
				q.push(pos);

				while (q.size()) {
					int pos = q.front(); q.pop();
					groupID[pos] = groupCount;
					auto [row, col] = decode(pos);
					count++;
					for (int i = 0; i < 4; i++) {
						int nrow = row + dx[i],
								ncol = col + dy[i];
						if (isOutside(nrow, ncol)) continue;
						int npos = encode(nrow, ncol);
						npos = find(npos);
						if (_groups[npos].valid) {
							if (_groups[npos].color == color) {
								touchedColor = true;
							} else {
								touchedOther = true;
							}
						} else if (!visited[npos]) {
							visited[npos] = true;
							q.push(npos);
						}
					}
				}
				groupSize[groupCount] = count;
				if (!touchedColor && touchedOther && count <= GR_SIZE_LIM) {
					owner[groupCount] = oppColor;
				} else if (touchedColor && !touchedOther && count <= GR_SIZE_LIM) {
					owner[groupCount] = color;
				}
			}

			int id = groupID[pos];

			if (!owner[id] || groupSize[id] > GR_SIZE_LIM) {
				validMoves.push_back({row, col});
			}
		}
	}
	return validMoves;
}

bool GroupManager::applyMove(int row, int col, int color) {
	auto [isValid, captured] = makeMove(row, col, color);
	return isValid;
}

void GroupManager::rollBack(int groupChangeCount, int libertyChangeCount) {
	for (; _groupChanges.size() && groupChangeCount; groupChangeCount--) {
		auto [node, lab, group] = _groupChanges.top(); _groupChanges.pop();
		_lab[node] = lab;
		_groups[node] = group;
	}
	for (; _libertyChanges.size() && libertyChangeCount; libertyChangeCount--) {
		auto [root, value, wasInsert] = _libertyChanges.top(); _libertyChanges.pop();
		if (wasInsert) {
			_groups[root].liberties.erase(value);
		} else {
			_groups[root].liberties.insert(value);
		}
	}
}

void GroupManager::undo() {
	if (_history.empty()) return;
	auto [groupChangeCount, libertyChangeCount] = _history.top(); _history.pop();
	rollBack(_groupChangeCount - groupChangeCount, _libertyChangeCount - libertyChangeCount);
  _groupChangeCount = groupChangeCount;
  _libertyChangeCount = libertyChangeCount;
}
