#include "SavedGameCommand.h"
#include "ICommand.h"
#include "GameModel.h"
#include "InGameScene.h"
#include <iostream>

// Select command: sets selected file in model
class SelectSavedGameCommand : public ICommand {
public:
    SelectSavedGameCommand(GameModel* m, std::string name) : _m(m), _name(std::move(name)) {}
    void execute() override {
        if (_m) _m->setSelectedGameData(_name);
    }
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}
    std::string getName() const override { return "SelectSavedGame"; }
    std::unique_ptr<ICommand> clone() const override { return std::make_unique<SelectSavedGameCommand>(_m, _name); }
    CommandType getType() const override { return CommandType::IMMEDIATE; }
private:
    GameModel* _m;
    std::string _name;
};

std::unique_ptr<ICommand> createSelectSavedGameCommand(GameModel* model, const std::string& filename) {
    return std::make_unique<SelectSavedGameCommand>(model, filename);
}

// Delete selected
class DeleteSelectedCommand : public ICommand {
public:
    DeleteSelectedCommand(GameModel* m) : _m(m) {}
    void execute() override {
        if (!_m) return;
        if (!_m->getSelectedGameData().has_value()) return;
        bool ok = _m->deleteSelectedFile();
        if (!ok) std::cerr << "Delete failed\n";
    }
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}
    std::string getName() const override { return "DeleteSelected"; }
    std::unique_ptr<ICommand> clone() const override { return std::make_unique<DeleteSelectedCommand>(_m); }
    CommandType getType() const override { return CommandType::IMMEDIATE; }
private:
    GameModel* _m;
};

std::unique_ptr<ICommand> createDeleteSelectedCommand(GameModel* model) {
    return std::make_unique<DeleteSelectedCommand>(model);
}

// Save to selected
class SaveSelectedCommand : public ICommand {
public:
    SaveSelectedCommand(GameModel* m) : _m(m) {}
    void execute() override {
        if (!_m) return;
        if (!_m->getSelectedGameData().has_value()) return;
        bool ok = _m->saveCurrentToSelectedFile();
        if (!ok) std::cerr << "Save to selected failed\n";
    }
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}
    std::string getName() const override { return "SaveToSelected"; }
    std::unique_ptr<ICommand> clone() const override { return std::make_unique<SaveSelectedCommand>(_m); }
    CommandType getType() const override { return CommandType::IMMEDIATE; }
private:
    GameModel* _m;
};

std::unique_ptr<ICommand> createSaveToSelectedCommand(GameModel* model) {
    return std::make_unique<SaveSelectedCommand>(model);
}

// Open create input (tell scene to open input)
class OpenCreateInputCommand : public ICommand {
public:
    OpenCreateInputCommand(InGameScene* s) : _s(s) {}
    void execute() override {
        if (_s) _s->openGameDataInputPopup();
    }
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}
    std::string getName() const override { return "OpenCreateInput"; }
    std::unique_ptr<ICommand> clone() const override { return std::make_unique<OpenCreateInputCommand>(_s); }
    CommandType getType() const override { return CommandType::IMMEDIATE; }
private:
    InGameScene* _s;
};

std::unique_ptr<ICommand> createOpenCreateInputCommand(InGameScene* scene) {
    return std::make_unique<OpenCreateInputCommand>(scene);
}

// Close saved list (tell scene to close popup)
class CloseCreateInputCommand : public ICommand {
public:
    CloseCreateInputCommand(InGameScene* s) : _s(s) {}
    void execute() override {
        if (_s) _s->closeGameDataInputPopup();
    }
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}
    std::string getName() const override { return "CloseCreateInput"; }
    std::unique_ptr<ICommand> clone() const override { return std::make_unique<CloseCreateInputCommand>(_s); }
    CommandType getType() const override { return CommandType::IMMEDIATE; }
private:
    InGameScene* _s;
};

std::unique_ptr<ICommand> createCloseCreateInputCommand(InGameScene* scene) {
    return std::make_unique<CloseCreateInputCommand>(scene);
}

// Close saved list (tell scene to close popup)
class CloseSavedListCommand : public ICommand {
public:
    CloseSavedListCommand(InGameScene* s) : _s(s) {}
    void execute() override {
        if (_s) _s->closeSavedGameListPopup();
    }
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}
    std::string getName() const override { return "CloseSavedList"; }
    std::unique_ptr<ICommand> clone() const override { return std::make_unique<CloseSavedListCommand>(_s); }
    CommandType getType() const override { return CommandType::IMMEDIATE; }
private:
    InGameScene* _s;
};

std::unique_ptr<ICommand> createCloseSavedListCommand(InGameScene* scene) {
    return std::make_unique<CloseSavedListCommand>(scene);
}

class OpenSavedListCommand : public ICommand {
public:
    OpenSavedListCommand(InGameScene* s) : _s(s) {}
    void execute() override {
        if (_s) _s->openSavedGameListPopup();
    }
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}
    std::string getName() const override { return "OpenSavedList"; }
    std::unique_ptr<ICommand> clone() const override { return std::make_unique<OpenSavedListCommand>(_s); }
    CommandType getType() const override { return CommandType::IMMEDIATE; }
private:
    InGameScene* _s;
};

std::unique_ptr<ICommand> createOpenSavedListCommand(InGameScene* scene) {
    return std::make_unique<OpenSavedListCommand>(scene);
}

class SavedGameSelectCommand : public ICommand {
public:
    SavedGameSelectCommand(GameModel* model, const std::string& file)
        : _model(model), _filename(file) {}

    void execute() override {
        if (_model) {
						_model->setSelectedGameData(_filename);
            std::cout << "Selected saved game: " << _filename << "\n";
        }
    }

    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}

    std::string getName() const override { return "SavedGameSelect"; }

    std::unique_ptr<ICommand> clone() const override {
        return std::make_unique<SavedGameSelectCommand>(_model, _filename);
    }

    CommandType getType() const override { return CommandType::IMMEDIATE; }

private:
    GameModel* _model;
    std::string _filename;
};

std::unique_ptr<ICommand> createSavedGameSelectCommand(
        GameModel* model, const std::string& filename)
{
    return std::make_unique<SavedGameSelectCommand>(model, filename);
}

class SaveFromInputCommand : public ICommand {
public:
    SaveFromInputCommand(GameModel* model, const std::string& filename)
        : _model(model), _filename(filename) {}

    void execute() override {
        if (!_model) return;
        _model->createNewSaveFile(_filename);
    }

    std::string getName() const override { return "Save from input Command"; }
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}
    std::unique_ptr<ICommand> clone() const override {
        return std::make_unique<SaveFromInputCommand>(_model, _filename);
    }
    CommandType getType() const override { return CommandType::IMMEDIATE; }

private:
    GameModel* _model;
		std::string _filename;
};

std::unique_ptr<ICommand> createSaveFromInputCommand(
        GameModel* model, const std::string& filename)
{
    return std::make_unique<SaveFromInputCommand>(model, filename);
}
