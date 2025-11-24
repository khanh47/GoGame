#pragma once
#include "ICommand.h"
#include "GameModel.h"

class GameCommand : public ICommand {
public:
    GameCommand(GameModel* gameModel)
        : _gameModel(gameModel) {}

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
    GameModel* _gameModel;
};

class UndoCommand : public GameCommand {
public:
    UndoCommand(GameModel* model)
        : GameCommand(model) {}

    void execute() override;

    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }

    std::string getName() const override { return "Undo Command"; }

    std::unique_ptr<ICommand> clone() const override {
        auto cloned = std::make_unique<UndoCommand>(_gameModel);
        cloned->_callback = _callback;
        return cloned;
    }
};

class RedoCommand : public GameCommand {
public:
    RedoCommand(GameModel* model)
        : GameCommand(model) {}

    void execute() override;

    std::string getName() const override { return "Redo Command"; }

    std::unique_ptr<ICommand> clone() const override {
        auto cloned = std::make_unique<RedoCommand>(_gameModel);
        cloned->_callback = _callback;
        return cloned;
    }
};

class ResetCommand : public GameCommand {
public:
    ResetCommand(GameModel* model)
        : GameCommand(model) {}

    void execute() override;

    std::string getName() const override { return "Reset Command"; }

    std::unique_ptr<ICommand> clone() const override {
        auto cloned = std::make_unique<ResetCommand>(_gameModel);
        cloned->_callback = _callback;
        return cloned;
    }
};

class SaveCommand : public GameCommand {
public:
    SaveCommand(GameModel* model, const std::string& file)
        : GameCommand(model), _filename(file) {}

    void execute() override;

    std::string getName() const override { return "Save Command"; }

    std::unique_ptr<ICommand> clone() const override {
        auto cloned = std::make_unique<SaveCommand>(_gameModel, _filename);
        cloned->_callback = _callback;
        return cloned;
    }

private:
    std::string _filename;
};

class PassCommand : public GameCommand {
public:
    PassCommand(GameModel* model)
        : GameCommand(model) {}

    void execute() override;

    std::string getName() const override { return "Pass Command"; }

    std::unique_ptr<ICommand> clone() const override {
        auto cloned = std::make_unique<PassCommand>(_gameModel);
        cloned->_callback = _callback;
        return cloned;
    }
};
