#pragma once

#include "BoardSizeController.h"
#include "ExpandableMenuItemView.h"
#include <memory>
#include <vector>

class GameSettings {
public:
    GameSettings();
    ~GameSettings();
    
    void handleInput();
    void update();
    void render() const;
    
    void setPosition(Vector2 position);
    void setSize(Vector2 size);
    float getTotalHeight() const;

private:
    void initializeMenuItems();
    void updateMenuItemPositions();
    
    std::unique_ptr<BoardSizeController> _boardSizeController;
    std::vector<std::unique_ptr<ExpandableMenuItemView>> _menuItems;
    
    Vector2 _position = {0, 0};
    Vector2 _size = {400, 300};
};