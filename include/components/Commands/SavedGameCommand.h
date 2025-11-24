#pragma once
#include "ICommand.h"
#include <memory>
#include <string>

class GameModel;
class InGameScene;

std::unique_ptr<ICommand> createSelectSavedGameCommand(GameModel* model, const std::string& filename);
std::unique_ptr<ICommand> createDeleteSelectedCommand(GameModel* model);
std::unique_ptr<ICommand> createSaveToSelectedCommand(GameModel* model);
std::unique_ptr<ICommand> createOpenCreateInputCommand(InGameScene* scene);
std::unique_ptr<ICommand> createCloseCreateInputCommand(InGameScene* scene);
std::unique_ptr<ICommand> createCloseSavedListCommand(InGameScene* scene);
std::unique_ptr<ICommand> createOpenSavedListCommand(InGameScene* scene);
std::unique_ptr<ICommand> createSavedGameSelectCommand(GameModel* model, const std::string& filename);
std::unique_ptr<ICommand> createSaveFromInputCommand(GameModel* model, const std::string& filename);
