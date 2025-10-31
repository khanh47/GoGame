#include "MenuCommand.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameState.h"
#include "ConcreteGameStates/GameModeState.h"
#include "ConcreteGameStates/InGameState.h"

// MenuCommand Implementation
void MenuCommand::execute()
{
    if (_gameStateModel && _stateFactory) {
        std::unique_ptr<GameState> newState = _stateFactory();
        if (_sceneManager && newState) {
            std::unique_ptr<Scene> newScene = newState->createScene();
            if (newScene) {
                _sceneManager->changeScene(std::move(newScene));
            }
        }
        _gameStateModel->setState(std::move(newState));
    }
}

std::unique_ptr<MenuCommand> createNewGameCommand(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    return std::make_unique<MenuCommand>(
        gameStateModel,
        sceneManager,
        std::function<std::unique_ptr<GameState>()>([]() { 
            return std::make_unique<GameModeState>(); 
        }),
        "New Game"
    );
}

std::unique_ptr<MenuCommand> createPlayCommand(GameStateModel *gameStateModel, SceneManager *sceneManager, const std::string& gameMode)
{
    return std::make_unique<MenuCommand>(
        gameStateModel,
        sceneManager,
        std::function<std::unique_ptr<GameState>()>([gameMode]() {
            return std::make_unique<InGameState>(gameMode);
        }),
        "Game Mode Play Command"
    );
}

// GameModeSelectCommand Implementation
void GameModeSelectCommand::execute() {
    if (_gameModeScene) {
        std::cout << "Selected game mode: " << _gameMode << std::endl;
        _gameModeScene->selectGameMode(_gameMode);
    }
    executeCallback();
}

std::unique_ptr<ICommand> GameModeSelectCommand::clone() const {
    auto cloned = std::make_unique<GameModeSelectCommand>(_gameMode, _gameModeScene);
    cloned->_callback = _callback; // Copy the callback
    return cloned;
}

// Exit Command Implementation
std::unique_ptr<ICommand> ExitCommand::clone() const
{
    return std::make_unique<ExitCommand>();
}

std::unique_ptr<ICommand> createExitCommand()
{
    return std::make_unique<ExitCommand>();
}
