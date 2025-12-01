#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>

// Encode (row, col) into a single integer for hashing
inline int encodePos(int r, int c, int cols) { return r * cols + c; }
inline std::pair<int,int> decodePos(int code, int cols) { return {code / cols, code % cols}; }

struct GroupInfo {
    int color;                          // 1 = black, 2 = white, 0 = empty/invalid
    int size;                           // number of stones in group
    std::unordered_set<int> liberties;  // set of encoded liberty positions
    std::unordered_set<int> members;    // set of encoded member positions
};

class GroupManager {
public:
    GroupManager(int rows, int cols);
    ~GroupManager() = default;

    void reset();

    // DSU operations
    int find(int x);
    void unite(int x, int y);

    // Core move operations
    // Returns: pair<isLegal, capturedStones>
    // isLegal = false means self-capture (illegal move)
    // capturedStones = list of (row,col) positions that were captured
    std::pair<bool, std::vector<std::pair<int,int>>> makeMove(int row, int col, int color);

    // Undo a move (for AI search) - requires the MoveRecord from makeMove
    struct MoveRecord {
        int row, col, color;
        std::vector<std::pair<int,int>> capturedStones;
        // Store previous DSU state for affected groups (simplified: we'll use full restore)
        std::vector<std::pair<int, GroupInfo>> previousGroups;
        std::vector<int> previousParent;
        std::vector<int> previousRank;
        std::vector<int> previousGroupId;
    };

    MoveRecord prepareMoveRecord(int row, int col, int color);
    bool applyMove(MoveRecord& record);  // returns false if illegal (self-capture)
    void undoMove(const MoveRecord& record);

    // Query functions
    int getGroupId(int row, int col);
    int getGroupLiberties(int row, int col);
    int getGroupSize(int row, int col);
    bool hasLiberties(int row, int col);

    // For territory calculation
    int countTerritory(int color);  // returns territory count for given color

    // Generate candidate moves (for AI)
    std::vector<std::pair<int,int>> generateCandidates(int radius = 2);

    // Check if a move would be self-capture (without actually making it)
    bool wouldBeSelfCapture(int row, int col, int color);

    // Get current board state
    int getValue(int row, int col) const { return _board[row][col]; }
    void setValue(int row, int col, int val) { _board[row][col] = val; }

    int getRows() const { return _rows; }
    int getCols() const { return _cols; }

private:
    int _rows, _cols;

    // Board state
    std::vector<std::vector<int>> _board;  // 0=empty, 1=black, 2=white

    // DSU arrays
    std::vector<int> _parent;
    std::vector<int> _rank;

    // Group ID for each cell (maps to root of DSU)
    std::vector<int> _groupId;  // encoded position -> group root

    // Group info indexed by group root
    std::unordered_map<int, GroupInfo> _groups;

    int _nextGroupId;

    // Direction arrays
    static constexpr int dx[4] = {0, 0, -1, 1};
    static constexpr int dy[4] = {-1, 1, 0, 0};

    // Helper functions
    bool isInside(int r, int c) const { return r >= 0 && r < _rows && c >= 0 && c < _cols; }
    int encode(int r, int c) const { return r * _cols + c; }
    std::pair<int,int> decode(int code) const { return {code / _cols, code % _cols}; }

    void createNewGroup(int row, int col, int color);
    void mergeGroups(int root1, int root2);
    void removeGroup(int root, std::vector<std::pair<int,int>>& removedStones);
    void updateLibertiesAfterCapture(const std::vector<std::pair<int,int>>& capturedStones);
    void computeLibertiesForGroup(int root);
};