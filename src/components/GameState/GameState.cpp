#include "GameState.h"
#include "ConcreteGameStates/InGameState.h"
#include "components/Scene/SceneManager.h"

// GameStateModel Implementation
GameStateModel::GameStateModel(){
    // Initialize with a default state, e.g., MainMenuState
    _currentState = std::make_unique<InGameState>();
    _currentState->onEnter(this);
}

void GameStateModel::setState(std::unique_ptr<GameState> newState){
    if (_currentState){
        _currentState->onExit(this);
    }
    _currentState = std::move(newState);
    if (_currentState){
        _currentState->onEnter(this);
    }
}

void GameStateModel::setStateByName(const std::string& stateName){
    auto newState = createState(stateName);
    if (newState){
        setState(std::move(newState));
    }
}

std::string GameStateModel::getCurrentStateName() const{
    if (_currentState){
        return _currentState->getName();
    }
    return "No State";
}

void GameStateModel::update(float deltaTime){
    if (_currentState){
        _currentState->update(this, deltaTime);
    }
}

std::unique_ptr<GameState> GameStateModel::createState(const std::string& stateName){
    if (stateName == "IN_GAME"){
        return std::make_unique<InGameState>();
    }
    // else if (stateName == "MainMenuState"){
    //     return std::make_unique<MainMenuState>();
    // }
    // Add other states as needed
    return nullptr;
}
