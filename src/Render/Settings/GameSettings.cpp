#include "GameSettings.h"

GameSettings::GameSettings() {
    _boardSizeController = std::make_unique<BoardSizeController>();
    initializeMenuItems();
}

GameSettings::~GameSettings() = default;

void GameSettings::initializeMenuItems() {
    _menuItems.clear();
    
    // Board Size item
    auto boardSizeItem = std::make_unique<ExpandableMenuItemView>(
        Vector2{_position.x, _position.y},
        Vector2{_size. x, 40.0f}, 
        "Board Size"
    );
    boardSizeItem->setExpandedContentSize({_size.x, 60.0f});
    boardSizeItem->setRenderExpandedContent(
        [this](Vector2 pos, Vector2 size) { 
            _boardSizeController->render(pos, size); 
        }
    );
    _menuItems.push_back(std::move(boardSizeItem));
    
    updateMenuItemPositions();
}

void GameSettings::handleInput() {
    Vector2 mousePos = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    
    for (size_t i = 0; i < _menuItems.size(); i++) {
        auto &item = _menuItems[i];
        
        bool isHeaderHovered = item->isHeaderPointed(mousePos);
        item->setHovered(isHeaderHovered);
        
        if (mouseClicked && isHeaderHovered) {
            item->toggleExpanded();
            updateMenuItemPositions();
        }
        
        if (item->isExpanded()) {
            Vector2 contentPos = {item->getPosition().x, item->getPosition(). y + item->getSize().y};
            Vector2 contentSize = {_size.x, 60.0f};
            
            if (i == 0) {
                _boardSizeController->handleInput(contentPos, contentSize);
            }
        }
    }
}

void GameSettings::update() {
    // Update logic if needed
}

void GameSettings::render() const {
    for (const auto &item : _menuItems) {
        item->render();
    }
}

void GameSettings::setPosition(Vector2 position) {
    _position = position;
    updateMenuItemPositions();
}

void GameSettings::setSize(Vector2 size) {
    _size = size;
    for (auto &item : _menuItems) {
        item->setSize({size.x, 40.0f});
        item->setExpandedContentSize({size.x, 60.0f});
    }
    updateMenuItemPositions();
}

float GameSettings::getTotalHeight() const {
    float totalHeight = 0;
    for (const auto &item : _menuItems) {
        totalHeight += item->getTotalHeight() + 5.0f;
    }
    return totalHeight;
}

void GameSettings::updateMenuItemPositions() {
    float currentY = _position.y;
    for (auto &item : _menuItems) {
        item->setPosition({_position.x, currentY});
        currentY += item->getTotalHeight() + 5.0f;
    }
}