#pragma once
#include "Scene.h"
#include "SettingsMenuController.h"
#include <string>

class AudioManager;
class SceneManager;

class SettingsScene : public Scene {
private:
	AudioManager *_audioManager;
	SceneManager *_sceneManager;
	std::unique_ptr<SettingsMenuController> menuController;
	bool _isActive = false;
	bool _shouldPopScene = false;

public:
	SettingsScene() = default;
	~SettingsScene() override = default;

	void setDependencies(AudioManager *audioManager, SceneManager *sceneManager);
	void init(void) override;
	void handleInput(void) override;
	void update(float deltaTime) override;
	void render(void) override;
	void cleanup(void) override;

	bool isActive(void) const override;
	std::string getName(void) const override;
	std::string getGameStateName(void) const override;

	void onEnter(void) override;
	void onExit(void) override;

	bool shouldTransition(void) const override;

	void requestPopScene();
	AudioManager *getAudioManager() const { return _audioManager; }

private:
	void initializeMenuController();
};
