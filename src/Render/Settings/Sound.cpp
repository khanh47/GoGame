#include "Sound.h"
#include "ExpandableMenuItemView.h"
#include "SoundEffectsController.h"
#include "MusicController.h"
#include "AudioManager.h"
#include "ResourceManager.h"

SoundSettings::SoundSettings(AudioManager* audioManager)
    : _audioManager(audioManager) {
    _soundEffectsController = std::make_unique<SoundEffectsController>(audioManager);
    _musicController = std::make_unique<MusicController>(audioManager);
    initializeMenuItems();
}

SoundSettings::~SoundSettings() = default;

void SoundSettings::initializeMenuItems() {
    _menuItems.clear();

    // Sound Effects item
    auto soundEffectsItem = std::make_unique<ExpandableMenuItemView>(
        Vector2{_position.x, _position.y}, 
        Vector2{_size.x, 40.0f}, 
        "Sound Effects"
    );
    soundEffectsItem->setExpandedContentSize({_size.x, 60.0f});
    soundEffectsItem->setRenderExpandedContent([this](Vector2 pos, Vector2 size) {
        _soundEffectsController->render(pos, size);
    });
    _menuItems.push_back(std::move(soundEffectsItem));

    // Background Music item
    auto musicItem = std::make_unique<ExpandableMenuItemView>(
        Vector2{_position.x, _position.y + 45.0f}, 
        Vector2{_size.x, 40.0f}, 
        "Background Music"
    );
    musicItem->setExpandedContentSize({_size.x, 60.0f});
    musicItem->setRenderExpandedContent([this](Vector2 pos, Vector2 size) {
        _musicController->render(pos, size);
    });
    _menuItems.push_back(std::move(musicItem));

    updateMenuItemPositions();
}

void SoundSettings::handleInput() {
    Vector2 mousePos = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    for (size_t i = 0; i < _menuItems.size(); i++) {
        auto& item = _menuItems[i];
        
        // Update hover state for header only
        bool isHeaderHovered = item->isHeaderPointed(mousePos);
        item->setHovered(isHeaderHovered);

        // Handle click on header to toggle expansion
        if (mouseClicked && isHeaderHovered) {
            item->toggleExpanded();
            updateMenuItemPositions();
        }

        // Handle input for expanded content
        if (item->isExpanded()) {
            Vector2 contentPos = {item->getPosition().x, item->getPosition().y + item->getSize().y};
            Vector2 contentSize = {_size.x, 60.0f};
            
            if (i == 0) {
                _soundEffectsController->handleInput(contentPos, contentSize);
            } else if (i == 1) {
                _musicController->handleInput(contentPos, contentSize);
            }
        }
    }
}

void SoundSettings::update() {
    // Update logic if needed
}

void SoundSettings::render() const {
    for (const auto& item : _menuItems) {
        item->render();
    }
}

void SoundSettings::setPosition(Vector2 position) {
    _position = position;
    updateMenuItemPositions();
}

void SoundSettings::setSize(Vector2 size) {
    _size = size;
    for (auto& item : _menuItems) {
        item->setSize({size.x, 40.0f});
        item->setExpandedContentSize({size.x, 60.0f});
    }
    updateMenuItemPositions();
}

float SoundSettings::getTotalHeight() const {
    float totalHeight = 0;
    for (const auto& item : _menuItems) {
        totalHeight += item->getTotalHeight() + 5.0f; // Add spacing
    }
    return totalHeight;
}

void SoundSettings::updateMenuItemPositions() {
    float currentY = _position.y;
    for (auto& item : _menuItems) {
        item->setPosition({_position.x, currentY});
        currentY += item->getTotalHeight() + 5.0f; // Add spacing between items
    }
}

