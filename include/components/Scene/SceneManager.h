#pragma once
#include <memory>
#include <stack>

// Forward declaration
class Scene;
class GameStateModel;

class SceneManager {
public:
    SceneManager(GameStateModel* gameStateModel);
    ~SceneManager();

    // Scene management
    void pushScene(std::unique_ptr<Scene> scene);
    void popScene();
    void changeScene(std::unique_ptr<Scene> scene);
    
    // Core loop methods
    void update(float deltaTime);
    void render();

    // Scene queries
    Scene* getCurrentScene() const;
    bool isEmpty() const;

    // Game state model access
    GameStateModel* getGameStateModel() const;

private:
    std::stack<std::unique_ptr<Scene>> _sceneStack;
    GameStateModel* _gameStateModel;

    std::unique_ptr<Scene> _nextScene;

    // Transition handling
    //  Handles the transition between scenes, including pushing and popping scenes
    bool _pendingTransition = false;
    bool _isChangeScene = false;


    void processTransitions(); // helper function to handle scene transitions, called in update
    std::unique_ptr<Scene> createScene(const std::string& name);

    // Menu system management
    
};