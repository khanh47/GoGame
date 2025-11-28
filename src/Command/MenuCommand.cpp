#include "MenuCommand.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameState.h"
#include "GameModeState.h"
#include "InGameState.h"
#include "MainMenuState.h"
#include "SettingsState.h"
#include "GameDataState.h"

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
        "New Game Command"
    );
}

std::unique_ptr<MenuCommand> createGameModeBackCommand(GameStateModel *gameStateModel, SceneManager *sceneManager) {
    return std::make_unique<MenuCommand>(
        gameStateModel,
        sceneManager,
        std::function<std::unique_ptr<GameState>()>([]() {
            return std::make_unique<MainMenuState>();
        }),
        "Back Command"
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

std::unique_ptr<MenuCommand> createInGameBackCommand(GameStateModel *gameStateModel, SceneManager *sceneManager) {
    return std::make_unique<MenuCommand>(
        gameStateModel,
        sceneManager,
        std::function<std::unique_ptr<GameState>()>([]() {
            return std::make_unique<MainMenuState>();
        }),
        "Back Command"
    );
}

std::unique_ptr<MenuCommand> createSettingsCommand(GameStateModel *gameStateModel, SceneManager *sceneManager) {
    return std::make_unique<MenuCommand>(
        gameStateModel,
        sceneManager,
        std::function<std::unique_ptr<GameState>()>([]() {
            return std::make_unique<SettingsState>();
        }),
        "Settings Command"
    );
}

std::unique_ptr<MenuCommand> createLoadGameCommand(GameStateModel* gameStateModel, SceneManager* sceneManager) {
		return std::make_unique<MenuCommand>(
				gameStateModel,
				sceneManager,
				std::function<std::unique_ptr<GameState>()>([]() {
						return std::make_unique<GameDataState>();
				}),
				"Load Game Command"
		);
}

// PopSceneCommand Implementation
void PopSceneCommand::execute() {
		if (_sceneManager) {
				_sceneManager->popScene();
		}
}

std::unique_ptr<ICommand> PopSceneCommand::clone() const {
        auto cloned = std::make_unique<PopSceneCommand>(_sceneManager);
        cloned->_callback = _callback;
        return cloned;
}

std::unique_ptr<PopSceneCommand> createPopSceneCommand(SceneManager *sceneManager)
{
    return std::make_unique<PopSceneCommand>(sceneManager);
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

// GameDataSelectCommand Implementation
void GameDataSelectCommand::execute() {
    if (_gameDataScene) {
        std::cout << "Selected game data: " << _gameData << std::endl;
        _gameDataScene->selectGameData(_gameData);
    }
    executeCallback();
}

std::unique_ptr<ICommand> GameDataSelectCommand::clone() const {
    auto cloned = std::make_unique<GameDataSelectCommand>(_gameData, _gameDataScene);
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
