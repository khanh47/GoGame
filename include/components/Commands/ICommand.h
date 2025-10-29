#pragma once
#include <string>
#include <memory>
#include <functional>
#include <iostream>

enum class CommandType {
    STATE_CHANGING,    // Commands that change game state (break execution)
    NON_STATE,        // Commands that don't change state (can continue)
    IMMEDIATE         // Commands that need immediate execution (break but don't change state)
};

// Interface for command pattern
class ICommand {
public:
    virtual ~ICommand() = default;

    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual std::string getName() const = 0;
    virtual bool canUndo() const = 0;
    virtual bool canRedo() const = 0;

    virtual CommandType getCommandType() const = 0;
    virtual std::unique_ptr<ICommand> clone() const = 0;

protected:
    std::function<void()> _callback;

public:
    void setCallBack(const std::function<void()>& callback) {
        _callback = callback;
    }

protected: 
    void executeCallback() {
        if (_callback) {
            _callback();
        }
        else {
            std::cout << "No callback set for this command." << std::endl;
        }
    }
};