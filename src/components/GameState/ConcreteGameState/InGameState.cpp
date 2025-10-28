#include "GameState.h"
#include "GameStates/ConcreteGameStates/InGameState.h"
#include "ResourceManager.h"
#include "Scene/ConcreteScene/InGameScene.h"

void InGameState::onEnter(GameStateModel *context)
{
}

void InGameState::onExit(GameStateModel *context)
{
}

void InGameState::update(GameStateModel *context, float deltaTime)
{
}

std::string InGameState::getName() const
{
    return "IN_GAME";
}

std::unique_ptr<GameState> InGameState::clone() const
{
    return std::make_unique<InGameState>(*this);
}

std::unique_ptr<Scene> InGameState::createScene() const
{
    auto scene = std::make_unique<InGameScene>();
    return scene;
}
