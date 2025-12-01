#pragma once
#include "raylib.h"
#include <memory>
#include <vector>

// Forward declarations
class ExpandableMenuItemView;
class BoardController;

class Theme {
public:
    Theme();
    ~Theme();

    void handleInput();
    void update();
    void render() const;

    void setPosition(Vector2 position);
    void setSize(Vector2 size);
    float getTotalHeight() const;

private:
    Vector2 _position = {0, 0};
    Vector2 _size = {400, 300};

    std::vector<std::unique_ptr<ExpandableMenuItemView>> _menuItems;
    std::unique_ptr<BoardController> _boardController;

    void initializeMenuItems();
    void updateMenuItemPositions();
};

