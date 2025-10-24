#pragma once
#include <string>

class Scene {
public:
    virtual ~Scene(void) = default;
    virtual void init(void) = 0;
    virtual void handleInput(void) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;

    // Scene state management
    virtual bool isActive() const = 0;
    virtual std::string getName(void) const = 0;
    virtual std::string getGameStateName(void) const = 0;

    // Scene transition methods
    virtual void onEnter(void) = 0;
    virtual void onExit(void) = 0;

    virtual bool shouldTransition(void) const { return false; }

protected:
    bool _isActive = false;
    float _transitionTimer = 0.0f;
};