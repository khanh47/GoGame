#pragma once
#include "GameController.h"
#include "ICommand.h"

class GameCommand : public ICommand {
public:
	GameCommand(GameController *gameModel) : _gameController(gameModel) {}

	// All logic commands override execute()
	void execute() override = 0;

	// Default: no undo/redo for basic commands
	virtual bool canUndo() const override { return false; }
	virtual bool canRedo() const override { return false; }
	virtual void undo() override {}
	virtual void redo() override {}

	std::string getName() const override { return "Game Command"; }

	std::unique_ptr<ICommand> clone() const override {
		// Base GameCommand shouldn't be cloned directly
		// Derived classes override clone()
		return nullptr;
	}

	CommandType getType() const override {
		return CommandType::IMMEDIATE; // logic happens instantly
	}

protected:
	GameController *_gameController;
};

class UndoCommand : public GameCommand {
public:
	UndoCommand(GameController *gameController) : GameCommand(gameController) {}

	void execute() override;

	bool canUndo() const override { return false; }
	bool canRedo() const override { return false; }

	std::string getName() const override { return "Undo Command"; }

	std::unique_ptr<ICommand> clone() const override {
		auto cloned = std::make_unique<UndoCommand>(_gameController);
		cloned->_callback = _callback;
		return cloned;
	}
};

class RedoCommand : public GameCommand {
public:
	RedoCommand(GameController *gameController) : GameCommand(gameController) {}

	void execute() override;

	std::string getName() const override { return "Redo Command"; }

	std::unique_ptr<ICommand> clone() const override {
		auto cloned = std::make_unique<RedoCommand>(_gameController);
		cloned->_callback = _callback;
		return cloned;
	}
};

class ResetCommand : public GameCommand {
public:
	ResetCommand(GameController *gameController) : GameCommand(gameController) {}

	void execute() override;

	std::string getName() const override { return "Reset Command"; }

	std::unique_ptr<ICommand> clone() const override {
		auto cloned = std::make_unique<ResetCommand>(_gameController);
		cloned->_callback = _callback;
		return cloned;
	}
};

class PassCommand : public GameCommand {
public:
	PassCommand(GameController *gameController) : GameCommand(gameController) {}

	void execute() override;

	std::string getName() const override { return "Pass Command"; }

	std::unique_ptr<ICommand> clone() const override {
		auto cloned = std::make_unique<PassCommand>(_gameController);
		cloned->_callback = _callback;
		return cloned;
	}
};
