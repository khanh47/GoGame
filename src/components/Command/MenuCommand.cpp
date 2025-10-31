#include "MenuCommand.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameState.h"
#include "ConcreteGameStates/InGameState.h"

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
    }
}

std::unique_ptr<ICommand> ExitCommand::clone() const
{
    return std::make_unique<ExitCommand>();
}

std::unique_ptr<MenuCommand> createNewGameCommand(GameStateModel *gameStateModel, SceneManager *sceneManager)
{
    return std::make_unique<MenuCommand>(
        gameStateModel,
        sceneManager,
        std::function<std::unique_ptr<GameState>()>([]() { 
            return std::make_unique<InGameState>(); 
        }),
        "New Game"
    );
}

std::unique_ptr<ICommand> createExitCommand()
{
    return std::make_unique<ExitCommand>();
}
