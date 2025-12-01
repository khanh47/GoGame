#include "AudioManager.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "raylib.h"

int main() {
	int screenWidth = 1200;
	int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "Go Game");
	InitAudioDevice();
	SetTargetFPS(60);

	ResourceManager &resourceManager = ResourceManager::getInstance();
	GameStateModel gamestate;
	AudioManager audioManager;
	SceneManager sceneManager(&gamestate, &audioManager);

	while (!WindowShouldClose()) {
		BeginDrawing();

		sceneManager.update(GetFrameTime());
		sceneManager.render();

		EndDrawing();
	}

	CloseAudioDevice();
	CloseWindow();
	return 0;
}
