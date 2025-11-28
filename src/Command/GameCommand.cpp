#include "GameCommand.h"

void UndoCommand::execute() {
		if (_gameController) {
				if (_gameController->undo()) {
						std::cout << "Undo successfully\n";
				} else {
						std::cout << "Undo failed\n";
				}
		}
}

void RedoCommand::execute() {
		if (_gameController) {
				if (_gameController->redo()) {
						std::cout << "Redo successfully\n";
				} else {
						std::cout << "Redo failed\n";
				}
		}
}

void ResetCommand::execute() {
		if (_gameController) {
				_gameController->resetGame();
		}
}

void PassCommand::execute() {
		if (_gameController) {
				_gameController->passGame();
		}
}
