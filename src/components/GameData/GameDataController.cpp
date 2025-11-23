//#include "GameDataController.h"
//#include "GameModel.h"
//#include <chrono>
//#include <fstream>
//#include <iostream>
//
//GameDataController::GameDataController(GameModel* model)
//    : _gameModel(model)
//{
//    _dataDir = std::filesystem::current_path() / "data";
//    if (!std::filesystem::exists(_dataDir)) {
//        std::filesystem::create_directories(_dataDir);
//    }
//
//    _history.clear();
//    _historyIndex = 0;
//
//    // initial snapshot
//    pushState(createSnapshot());
//}
//
//GameSnapShot GameDataController::createSnapshot() const {
//    GameSnapShot s;
//    s._grid = _gameModel->getGrid();
//    s._currentPlayer = _gameModel->getCurrentPlayer();
//    s._scorePlayer1 = _gameModel->getScorePlayer1();
//    s._scorePlayer2 = _gameModel->getScorePlayer2();
//    s._gameMode = _gameModel->getGameMode();
//    return s;
//}
//
//void GameDataController::applySnapshot(const GameSnapShot& snap) {
//    _gameModel->loadFromSnapshot(
//        snap._grid,
//        snap._currentPlayer,
//        snap._scorePlayer1,
//        snap._scorePlayer2,
//        snap._gameMode
//    );
//}
//
//void GameDataController::pushState(const GameSnapShot& snap) {
//    trimHistoryAfterIndex();
//    _history.push_back(snap);
//    _historyIndex = _history.size() - 1;
//}
//
//void GameDataController::pushResetState() {
//    pushState(createSnapshot());
//}
//
//void GameDataController::trimHistoryAfterIndex() {
//    if (_historyIndex + 1 < _history.size())
//        _history.erase(_history.begin() + _historyIndex + 1, _history.end());
//}
//
//bool GameDataController::canUndo() const { return _historyIndex > 0; }
//bool GameDataController::canRedo() const { return _historyIndex + 1 < _history.size(); }
//
//bool GameDataController::undo() {
//    if (!canUndo()) return false;
//    _historyIndex--;
//    applySnapshot(_history[_historyIndex]);
//    return true;
//}
//
//bool GameDataController::redo() {
//    if (!canRedo()) return false;
//    _historyIndex++;
//    applySnapshot(_history[_historyIndex]);
//    return true;
//}
//
