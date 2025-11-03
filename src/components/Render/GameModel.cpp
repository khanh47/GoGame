#include "GameModel.h"
#include "HUD.h"
#include "Game.h"

GameModel::GameModel()
{
    hud = std::make_shared<HUD>();
    game = std::make_shared<Game>(19, 19); // Standard Go board size
}

void GameModel::render() {
    if (game) {
        game->render();
    }
    if (hud) {
        hud->render();
    }
}

void GameModel::handleInput() {
    if (game) {
        game->handleInput();
        if (hud) update();
    }
}

void GameModel::update() {
    if (!game || !hud) return;
    int scorePlayer1 = game->getScorePlayer1();
    int scorePlayer2 = game->getScorePlayer2();
    int currentPlayer = game->getCurrentPlayer();
    hud->getScores(scorePlayer1, scorePlayer2, currentPlayer);
}
