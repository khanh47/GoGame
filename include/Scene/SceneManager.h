#pragma once
#include <memory>
#include <stack>

// Forward declaration
class Scene;
class GameStateModel;
class MenuComponent;
class NavigationMenuController;
class GameController;

class SceneManager {
public:
    SceneManager(GameStateModel* gameStateModel);
    ~SceneManager();

    // Scene management
    void pushScene(std::unique_ptr<Scene> scene);
    void popScene();
    void changeScene(std::unique_ptr<Scene> scene);
    void pushSceneDeferred(std::unique_ptr<Scene> scene); // push scene in next update cycle

    // Core loop methods
    void update(float deltaTime);
    void render();

    // Scene queries
    Scene* getCurrentScene() const;
    bool isEmpty() const;

    // Menu management
    void showMenu();
    void hideMenu();
    bool isMenuActive() const;
    void toggleMenu();
    void forceMenuRefresh(); // Force navigation menu to refresh

    // Game state gameController access
    GameStateModel* getGameStateModel() const;

private:
    std::stack<std::unique_ptr<Scene>> _sceneStack;
    GameStateModel* _gameStateModel;
    std::shared_ptr<NavigationMenuController> _navigationMenuController;
    std::shared_ptr<MenuComponent> _navigationMenuSystem;
    bool _menuActive = false;

    std::unique_ptr<Scene> _nextScene;

    // Transition handling
    //  Handles the transition between scenes, including pushing and popping scenes
    bool _pendingTransition = false;
    bool _isChangeScene = false;

    void processTransitions(); // helper function to handle scene transitions, called in update
    std::unique_ptr<Scene> createScene(const std::string& name);

    // Menu system management
    void initializeMenuSystem();
    void updateMenuSystem(float deltaTime);
    void renderMenuSystem();
};
