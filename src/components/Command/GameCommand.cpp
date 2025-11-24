#include "GameCommand.h"

void UndoCommand::execute() {
		if (_gameModel) {
				if (_gameModel->undo()) {
						std::cout << "Undo successfully\n";
				} else {
						std::cout << "Undo failed\n";
				}
		}
}

void RedoCommand::execute() {
		if (_gameModel) {
				if (_gameModel->redo()) {
						std::cout << "Redo successfully\n";
				} else {
						std::cout << "Redo failed\n";
				}
		}
}

void ResetCommand::execute() {
		if (_gameModel) {
				_gameModel->resetGame();
		}
}

void SaveCommand::execute() {
		if (_gameModel) {
				const GameSnapShot* snap = _gameModel->currentSnapShot();
				if (snap) {
						_gameModel->saveToFile(*snap, _filename);

				}
		}
}
