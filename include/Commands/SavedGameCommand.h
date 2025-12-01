#pragma once
#include "ICommand.h"
#include <memory>
#include <string>

class DataManager;
class InGameScene;

std::unique_ptr<ICommand> createSelectSavedGameCommand(DataManager* data, const std::string& filename);
std::unique_ptr<ICommand> createDeleteSelectedCommand(DataManager* data);
std::unique_ptr<ICommand> createSaveToSelectedCommand(DataManager* data);
std::unique_ptr<ICommand> createOpenCreateInputCommand(InGameScene* scene);
std::unique_ptr<ICommand> createCloseCreateInputCommand(InGameScene* scene);
std::unique_ptr<ICommand> createCloseAndCreateInputCommand(InGameScene* scene);
std::unique_ptr<ICommand> createCloseSavedListCommand(InGameScene* scene);
std::unique_ptr<ICommand> createOpenSavedListCommand(InGameScene* scene);
std::unique_ptr<ICommand> createSavedGameSelectCommand(DataManager* data, const std::string& filename);
