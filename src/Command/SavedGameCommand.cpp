#include "SavedGameCommand.h"
#include "ICommand.h"
#include "DataManager.h"
#include "InGameScene.h"
#include <iostream>

// Select command: sets selected file in data
class SelectSavedGameCommand : public ICommand {
public:
    SelectSavedGameCommand(DataManager* m, std::string name) : _m(m), _name(std::move(name)) {}
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
    DataManager* _m;
    std::string _name;
};

std::unique_ptr<ICommand> createSelectSavedGameCommand(DataManager* data, const std::string& filename) {
    return std::make_unique<SelectSavedGameCommand>(data, filename);
}

// Delete selected
class DeleteSelectedCommand : public ICommand {
public:
    DeleteSelectedCommand(DataManager* m) : _m(m) {}
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
    DataManager* _m;
};

std::unique_ptr<ICommand> createDeleteSelectedCommand(DataManager* data) {
    return std::make_unique<DeleteSelectedCommand>(data);
}

// Save to selected
class SaveSelectedCommand : public ICommand {
public:
    SaveSelectedCommand(DataManager* m) : _m(m) {}
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
    DataManager* _m;
};

std::unique_ptr<ICommand> createSaveToSelectedCommand(DataManager* data) {
    return std::make_unique<SaveSelectedCommand>(data);
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

// Close input bar (tell scene to close popup)
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

class CloseAndCreateInputCommand : public ICommand {
public:
    CloseAndCreateInputCommand(InGameScene* s) : _s(s) {}
    void execute() override {
        if (_s) _s->closeGameDataInputPopupAndCreate();
				std::cout << "Entered to create a new file\n";
    }
    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}
    std::string getName() const override { return "CloseAndCreateInput"; }
    std::unique_ptr<ICommand> clone() const override { return std::make_unique<CloseAndCreateInputCommand>(_s); }
    CommandType getType() const override { return CommandType::IMMEDIATE; }
private:
    InGameScene* _s;
};

std::unique_ptr<ICommand> createCloseAndCreateInputCommand(InGameScene* scene) {
    return std::make_unique<CloseAndCreateInputCommand>(scene);
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
    SavedGameSelectCommand(DataManager* data, const std::string& file)
        : _data(data), _filename(file) {}

    void execute() override {
        if (_data) {
						_data->setSelectedGameData(_filename);
            std::cout << "Selected saved game: " << _filename << "\n";
        }
    }

    bool canUndo() const override { return false; }
    bool canRedo() const override { return false; }
    void undo() override {}
    void redo() override {}

    std::string getName() const override { return "SavedGameSelect"; }

    std::unique_ptr<ICommand> clone() const override {
        return std::make_unique<SavedGameSelectCommand>(_data, _filename);
    }

    CommandType getType() const override { return CommandType::IMMEDIATE; }

private:
    DataManager* _data;
    std::string _filename;
};

std::unique_ptr<ICommand> createSavedGameSelectCommand(
        DataManager* data, const std::string& filename)
{
    return std::make_unique<SavedGameSelectCommand>(data, filename);
}
