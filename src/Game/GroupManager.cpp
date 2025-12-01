#include "GroupManager.h"
#include <queue>
#include <algorithm>

constexpr int GroupManager::dx[4];
constexpr int GroupManager::dy[4];

GroupManager::GroupManager(int rows, int cols)
    : _rows(rows), _cols(cols), _nextGroupId(1)
{
    reset();
}

void GroupManager::reset() {
    _board.assign(_rows, std::vector<int>(_cols, 0));
    _parent.assign(_rows * _cols, -1);
    _rank.assign(_rows * _cols, 0);
    _groupId.assign(_rows * _cols, -1);
    _groups.clear();
    _nextGroupId = 1;
}

// DSU Find with path compression
int GroupManager::find(int x) {
    if (_parent[x] < 0) return x;  // x is a root or uninitialized
    if (_parent[x] == x) return x;
    return _parent[x] = find(_parent[x]);  // path compression
}

// DSU Unite by rank
void GroupManager::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX == rootY) return;

    // Union by rank
    if (_rank[rootX] < _rank[rootY]) std::swap(rootX, rootY);
    _parent[rootY] = rootX;
    if (_rank[rootX] == _rank[rootY]) _rank[rootX]++;

    // Merge group info: merge rootY into rootX
    mergeGroups(rootX, rootY);
}

void GroupManager::createNewGroup(int row, int col, int color) {
    int pos = encode(row, col);
    _parent[pos] = pos;  // self-root
    _rank[pos] = 0;
    _groupId[pos] = pos;

    GroupInfo info;
    info.color = color;
    info. size = 1;
    info.members.insert(pos);

    // Compute initial liberties
    for (int k = 0; k < 4; k++) {
        int nr = row + dx[k], nc = col + dy[k];
        if (isInside(nr, nc) && _board[nr][nc] == 0) {
            info. liberties.insert(encode(nr, nc));
        }
    }

    _groups[pos] = std::move(info);
}

void GroupManager::mergeGroups(int rootKeep, int rootMerge) {
    if (! _groups.count(rootKeep) || !_groups. count(rootMerge)) return;
    if (rootKeep == rootMerge) return;

    GroupInfo& keep = _groups[rootKeep];
    GroupInfo& merge = _groups[rootMerge];

    // Merge members
    for (int pos : merge.members) {
        keep.members.insert(pos);
        _groupId[pos] = rootKeep;
    }
    keep.size += merge.size;

    // Merge liberties (union of both, minus any positions that are now members)
    for (int lib : merge.liberties) {
        if (keep.members.find(lib) == keep.members.end()) {
            keep. liberties.insert(lib);
        }
    }
    // Remove any liberties that are actually members
    for (int mem : keep.members) {
        keep.liberties.erase(mem);
    }

    // Remove old group
    _groups.erase(rootMerge);
}

void GroupManager::removeGroup(int root, std::vector<std::pair<int,int>>& removedStones) {
    if (! _groups.count(root)) return;

    GroupInfo& group = _groups[root];
    for (int pos : group.members) {
        auto [r, c] = decode(pos);
        _board[r][c] = 0;
        _parent[pos] = -1;
        _groupId[pos] = -1;
        removedStones. push_back({r, c});
    }
    _groups.erase(root);
}

void GroupManager::updateLibertiesAfterCapture(const std::vector<std::pair<int,int>>& capturedStones) {
    // For each captured stone position (now empty), add it as liberty to adjacent groups
    for (auto [r, c] : capturedStones) {
        int pos = encode(r, c);
        for (int k = 0; k < 4; k++) {
            int nr = r + dx[k], nc = c + dy[k];
            if (! isInside(nr, nc)) continue;
            if (_board[nr][nc] == 0) continue;

            int neighborPos = encode(nr, nc);
            int root = find(neighborPos);
            if (_groups.count(root)) {
                _groups[root].liberties.insert(pos);
            }
        }
    }
}

void GroupManager::computeLibertiesForGroup(int root) {
    if (!_groups.count(root)) return;
    GroupInfo& group = _groups[root];
    group.liberties.clear();

    for (int pos : group.members) {
        auto [r, c] = decode(pos);
        for (int k = 0; k < 4; k++) {
            int nr = r + dx[k], nc = c + dy[k];
            if (isInside(nr, nc) && _board[nr][nc] == 0) {
                group. liberties.insert(encode(nr, nc));
            }
        }
    }
}

std::pair<bool, std::vector<std::pair<int,int>>> GroupManager::makeMove(int row, int col, int color) {
    std::vector<std::pair<int,int>> capturedStones;

    if (! isInside(row, col) || _board[row][col] != 0) {
        return {false, capturedStones};  // invalid move
    }

    int pos = encode(row, col);
    int opp = (color == 1) ? 2 : 1;

    // 1) Place the stone
    _board[row][col] = color;
    createNewGroup(row, col, color);

    // 2) Remove this position from liberties of all adjacent groups
    for (int k = 0; k < 4; k++) {
        int nr = row + dx[k], nc = col + dy[k];
        if (! isInside(nr, nc)) continue;
        if (_board[nr][nc] == 0) continue;

        int neighborPos = encode(nr, nc);
        int root = find(neighborPos);
        if (_groups.count(root)) {
            _groups[root].liberties.erase(pos);
        }
    }

    // 3) Merge with adjacent friendly groups
    for (int k = 0; k < 4; k++) {
        int nr = row + dx[k], nc = col + dy[k];
        if (!isInside(nr, nc)) continue;
        if (_board[nr][nc] != color) continue;

        int neighborPos = encode(nr, nc);
        unite(pos, neighborPos);
    }

    // 4) Check adjacent opponent groups for capture
    std::unordered_set<int> checkedRoots;
    for (int k = 0; k < 4; k++) {
        int nr = row + dx[k], nc = col + dy[k];
        if (!isInside(nr, nc)) continue;
        if (_board[nr][nc] != opp) continue;

        int neighborPos = encode(nr, nc);
        int root = find(neighborPos);
        if (checkedRoots.count(root)) continue;
        checkedRoots.insert(root);

        if (_groups.count(root) && _groups[root].liberties.empty()) {
            // Capture this group
            removeGroup(root, capturedStones);
        }
    }

    // 5) Update liberties for groups adjacent to captured stones
    if (!capturedStones.empty()) {
        updateLibertiesAfterCapture(capturedStones);
    }

    // 6) Check if our newly placed group has liberties (self-capture check)
    int myRoot = find(pos);
    if (_groups.count(myRoot) && _groups[myRoot].liberties.empty()) {
        // Self-capture!  Undo placement
        std::vector<std::pair<int,int>> selfRemoved;
        removeGroup(myRoot, selfRemoved);

        // Restore captured opponent stones (if any)
        for (auto [r, c] : capturedStones) {
            _board[r][c] = opp;
            createNewGroup(r, c, opp);
        }
        // Re-merge restored opponent stones
        for (auto [r, c] : capturedStones) {
            int p = encode(r, c);
            for (int k = 0; k < 4; k++) {
                int nr2 = r + dx[k], nc2 = c + dy[k];
                if (!isInside(nr2, nc2)) continue;
                if (_board[nr2][nc2] != opp) continue;
                unite(p, encode(nr2, nc2));
            }
        }
        // Recompute liberties for restored groups
        std::unordered_set<int> restoredRoots;
        for (auto [r, c] : capturedStones) {
            restoredRoots.insert(find(encode(r, c)));
        }
        for (int root : restoredRoots) {
            computeLibertiesForGroup(root);
        }

        return {false, {}};  // illegal self-capture
    }

    return {true, capturedStones};
}

GroupManager::MoveRecord GroupManager::prepareMoveRecord(int row, int col, int color) {
    MoveRecord record;
    record.row = row;
    record.col = col;
    record. color = color;

    // Save full state (simplified approach - for complex undo)
    record.previousParent = _parent;
    record.previousRank = _rank;
    record. previousGroupId = _groupId;
    for (auto& [id, info] : _groups) {
        record. previousGroups.push_back({id, info});
    }

    return record;
}

bool GroupManager::applyMove(MoveRecord& record) {
    auto [legal, captured] = makeMove(record.row, record.col, record. color);
    record.capturedStones = std::move(captured);
    return legal;
}

void GroupManager::undoMove(const MoveRecord& record) {
    // Restore board
    _board[record.row][record. col] = 0;
    for (auto [r, c] : record.capturedStones) {
        int opp = (record. color == 1) ? 2 : 1;
        _board[r][c] = opp;
    }

    // Restore DSU state
    _parent = record.previousParent;
    _rank = record.previousRank;
    _groupId = record. previousGroupId;

    // Restore groups
    _groups.clear();
    for (auto& [id, info] : record.previousGroups) {
        _groups[id] = info;
    }
}

int GroupManager::getGroupId(int row, int col) {
    if (!isInside(row, col) || _board[row][col] == 0) return -1;
    return find(encode(row, col));
}

int GroupManager::getGroupLiberties(int row, int col) {
    int root = getGroupId(row, col);
    if (root < 0 || !_groups. count(root)) return 0;
    return _groups[root].liberties.size();
}

int GroupManager::getGroupSize(int row, int col) {
    int root = getGroupId(row, col);
    if (root < 0 || !_groups.count(root)) return 0;
    return _groups[root].size;
}

bool GroupManager::hasLiberties(int row, int col) {
    return getGroupLiberties(row, col) > 0;
}

bool GroupManager::wouldBeSelfCapture(int row, int col, int color) {
    if (!isInside(row, col) || _board[row][col] != 0) return true;

    int opp = (color == 1) ? 2 : 1;
    int pos = encode(row, col);

    // Check 1: Does placing here give us at least one liberty? 
    bool hasDirectLiberty = false;
    for (int k = 0; k < 4; k++) {
        int nr = row + dx[k], nc = col + dy[k];
        if (isInside(nr, nc) && _board[nr][nc] == 0) {
            hasDirectLiberty = true;
            break;
        }
    }
    if (hasDirectLiberty) return false;  // Not self-capture

    // Check 2: Does placing here connect to a friendly group with >1 liberty? 
    for (int k = 0; k < 4; k++) {
        int nr = row + dx[k], nc = col + dy[k];
        if (!isInside(nr, nc)) continue;
        if (_board[nr][nc] == color) {
            int root = find(encode(nr, nc));
            if (_groups. count(root) && _groups[root]. liberties.size() > 1) {
                return false;  // Connected group has other liberties
            }
        }
    }

    // Check 3: Does placing here capture an opponent group?
    for (int k = 0; k < 4; k++) {
        int nr = row + dx[k], nc = col + dy[k];
        if (!isInside(nr, nc)) continue;
        if (_board[nr][nc] == opp) {
            int root = find(encode(nr, nc));
            if (_groups.count(root) && _groups[root].liberties.size() == 1) {
                // This opponent group has only one liberty (this position)
                // Capturing it would give us liberties
                return false;
            }
        }
    }

    // All checks failed -> would be self-capture
    return true;
}

std::vector<std::pair<int,int>> GroupManager::generateCandidates(int radius) {
    std::vector<std::vector<char>> candidate(_rows, std::vector<char>(_cols, 0));
    bool hasStone = false;

    for (int i = 0; i < _rows && !hasStone; i++) {
        for (int j = 0; j < _cols && !hasStone; j++) {
            if (_board[i][j] != 0) hasStone = true;
        }
    }

    if (!hasStone) {
        // Empty board: return center
        return {{_rows / 2, _cols / 2}};
    }

    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            if (_board[i][j] != 0) {
                for (int di = -radius; di <= radius; di++) {
                    for (int dj = -radius; dj <= radius; dj++) {
                        int ni = i + di, nj = j + dj;
                        if (isInside(ni, nj) && _board[ni][nj] == 0) {
                            candidate[ni][nj] = 1;
                        }
                    }
                }
            }
        }
    }

    std::vector<std::pair<int,int>> moves;
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            if (candidate[i][j]) {
                moves. push_back({i, j});
            }
        }
    }

    if (moves.empty()) {
        // Fallback: all empty cells
        for (int i = 0; i < _rows; i++) {
            for (int j = 0; j < _cols; j++) {
                if (_board[i][j] == 0) {
                    moves.push_back({i, j});
                }
            }
        }
    }

    return moves;
}

int GroupManager::countTerritory(int color) {
    std::vector<std::vector<bool>> visited(_rows, std::vector<bool>(_cols, false));
    int territory = 0;

    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            if (visited[i][j] || _board[i][j] != 0) continue;

            // BFS to find empty region
            std::queue<std::pair<int,int>> q;
            std::vector<std::pair<int,int>> region;
            bool touchesColor = false;
            bool touchesOther = false;
            int other = (color == 1) ? 2 : 1;

            q.push({i, j});
            visited[i][j] = true;

            while (!q. empty()) {
                auto [r, c] = q.front();
                q.pop();
                region.push_back({r, c});

                for (int k = 0; k < 4; k++) {
                    int nr = r + dx[k], nc = c + dy[k];
                    if (! isInside(nr, nc)) continue;

                    if (_board[nr][nc] == 0 && !visited[nr][nc]) {
                        visited[nr][nc] = true;
                        q.push({nr, nc});
                    } else if (_board[nr][nc] == color) {
                        touchesColor = true;
                    } else if (_board[nr][nc] == other) {
                        touchesOther = true;
                    }
                }
            }

            if (touchesColor && ! touchesOther) {
                territory += region.size();
            }
        }
    }

    return territory;
}