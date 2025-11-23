//#pragma once
//#include <vector>
//#include <string>
//#include <optional>
//#include <filesystem>
//
//class GameModel;
//
//struct GameSnapShot {
//    std::vector<std::vector<int>> _grid;
//    int _currentPlayer = 1;
//    int _scorePlayer1 = 0;
//    int _scorePlayer2 = 0;
//    std::string _gameMode;
//
//    // metadata
//    std::string note;
//    uint64_t timestamp = 0;
//
//    void serialize(std::ostream& out) const;
//    bool deserialize(std::istream& in);
//};
//
//class GameDataController {
//private:
//    GameModel* _gameModel;   // reference to gameplay model
//
//    std::vector<GameSnapShot> _history;
//    size_t _historyIndex = 0;
//
//    std::filesystem::path _dataDir;
//
//    void trimHistoryAfterIndex();
//
//public:
//    GameDataController(GameModel* model);
//
//    // snapshots
//    GameSnapShot createSnapshot() const;
//    const GameSnapShot* currentSnapshot() const;
//
//    void applySnapshot(const GameSnapShot& snap);
//    void pushState(const GameSnapShot& snap);
//    void pushResetState();
//
//    bool canUndo() const;
//    bool canRedo() const;
//    bool undo();
//    bool redo();
//
//    // save/load
//    bool saveToFile(const GameSnapShot& snap, const std::string& filename);
//    std::optional<GameSnapShot> loadFromFile(const std::string& filename);
//
//    std::vector<std::string> getSavedGamesList() const;
//    bool deleteSavedGame(const std::string& filename);
//    std::optional<std::string> latestSavedGameFileName() const;
//};
//
