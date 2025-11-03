#include "GameModel.h"
#include "HUD.h"
#include "Game.h"

GameModel::GameModel()
{
    _hud = std::make_shared<HUD>();
    _game = std::make_shared<Game>(19, 19); // Standard Go board size
}

void GameModel::render() {
    if (!_game || !_hud) return;
    _game->render();
    _hud->render();
}

void GameModel::handleInput() {
    if (!_game || !_hud) return;
    _game->handleInput();
    update();
}

void GameModel::update() {
    if (!_game || !_hud) return;
    int scorePlayer1 = _game->getScorePlayer1();
    int scorePlayer2 = _game->getScorePlayer2();
    int currentPlayer = _game->getCurrentPlayer();
    _hud->getScores(scorePlayer1, scorePlayer2, currentPlayer);
}