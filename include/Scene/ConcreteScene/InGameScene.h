#pragma once
#include "EndGameBox.h"
#include "InGameMenuController.h"
#include "PassButton.h"
#include "SavedGameList.h"
#include "Scene.h"
#include "TextBox.h"
#include <memory>

// Forward declaration
class GameController;
class Game;
class AudioManager;

class InGameScene : public Scene {
public:
	InGameScene(const std::string &gameMode);
	~InGameScene() override = default;
	void setDependencies(AudioManager *audioManager);

	void init(void) override;
	void update(float deltaTime) override;
	void render(void) override;
	void handleInput(void) override;
	void cleanup(void) override;

	bool isActive(void) const override;
	std::string getName(void) const override;
	std::string getGameStateName(void) const override;

	void onEnter(void) override;
	void onExit(void) override;

	bool shouldTransition(void) const override;

	// popup control functions
	void openSavedGameListPopup();
	void closeSavedGameListPopup();

	void openGameDataInputPopup();
	void closeGameDataInputPopup();
	void closeGameDataInputPopupAndCreate();

	bool isPopup();

private:
	std::unique_ptr<InGameMenuController> menuController;
	std::unique_ptr<PassButton> _passButton;
	std::unique_ptr<TextBox> _textBox;
	std::unique_ptr<EndGameBox> _endGameBox;
	GameController *_gameController = nullptr;
	AudioManager *_audioManager = nullptr;
	std::string _gameModeSelected;
	bool _playedWinningSound = false;

private:
	void initializeMenuController();
	void initializePassButton();
};
