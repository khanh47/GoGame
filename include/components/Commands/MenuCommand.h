#pragma once
#include "GameState.h"
#include "ICommand.h"
#include <functional>

// Forawrd declarations
class GameState;
class GameStateModel;
class SceneManager;

class MenuCommand : public ICommand {
protected:
    GameStateModel* _gameStateModel;
    SceneManager* _sceneManager;
    std::function<std::unique_ptr<GameState>()> _stateFactory;
    std::string _commandName;

public:
    MenuCommand(GameStateModel* gameStateModel,
                SceneManager* sceneManager,
                std::function<std::unique_ptr<GameState>()> stateFactory,
                const std::string& commandName)
        : _gameStateModel(gameStateModel),
          _sceneManager(sceneManager),
          _stateFactory(stateFactory),
          _commandName(commandName) {}

    virtual ~MenuCommand() = default;

    virtual bool canUndo() const override { return false; }
    virtual bool canRedo() const override { return false; }
    virtual void undo() override {}
    virtual void redo() override {}
    virtual CommandType getType() const override { return CommandType::STATE_CHANGING; };
    virtual std::string getName() const override { return _commandName; }

    virtual void execute() override;
    virtual std::unique_ptr<ICommand> clone() const override {
        return std::make_unique<MenuCommand>(_gameStateModel, _sceneManager, _stateFactory, _commandName);
    }
};

// Factory functions for specific state switchers
std::unique_ptr<MenuCommand> createNewGameCommand(GameStateModel* gameStateModel, SceneManager* sceneManager);
// std::unique_ptr<MenuCommand> createLoadGameCommand(GameStateModel* gameStateModel, SceneManager* sceneManager);
// std::unique_ptr<MenuCommand> createSettingsCommand(GameStateModel* gameStateModel, SceneManager* sceneManager);

class ExitCommand : public ICommand {
public:
    ExitCommand() = default;
    void execute() override { exit(0); };

    virtual bool canUndo() const override { return false; }
    virtual bool canRedo() const override { return false; }
    virtual void undo() override {}
    virtual void redo() override {}
    virtual CommandType getType() const override { return CommandType::IMMEDIATE; };
    virtual std::string getName() const override { return "Exit Game"; }

    virtual std::unique_ptr<ICommand> clone() const override;
};

std::unique_ptr<ICommand> createExitCommand();