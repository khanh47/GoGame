#include "Theme.h"
#include "BoardController.h"
#include "ExpandableMenuItemView.h"
#include "ResourceManager.h"

Theme::Theme() {
	_boardController = std::make_unique<BoardController>();
	initializeMenuItems();
}

Theme::~Theme() = default;

void Theme::initializeMenuItems() {
	_menuItems.clear();

	// Board Design item
	auto boardItem = std::make_unique<ExpandableMenuItemView>(Vector2{_position.x, _position.y}, Vector2{_size.x, 40.0f},
																														"Board Design");
	boardItem->setExpandedContentSize({_size.x, 200.0f});
	boardItem->setRenderExpandedContent([this](Vector2 pos, Vector2 size) { _boardController->render(pos, size); });
	_menuItems.push_back(std::move(boardItem));

	updateMenuItemPositions();
}

void Theme::handleInput() {
	Vector2 mousePos = GetMousePosition();
	bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

	for (size_t i = 0; i < _menuItems.size(); i++) {
		auto &item = _menuItems[i];

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
			Vector2 contentSize = {_size.x, 200.0f};

			if (i == 0) {
				_boardController->handleInput(contentPos, contentSize);
			}
		}
	}
}

void Theme::update() {
	// Update logic if needed
}

void Theme::render() const {
	for (const auto &item : _menuItems) {
		item->render();
	}
}

void Theme::setPosition(Vector2 position) {
	_position = position;
	updateMenuItemPositions();
}

void Theme::setSize(Vector2 size) {
	_size = size;
	for (auto &item : _menuItems) {
		item->setSize({size.x, 40.0f});
		item->setExpandedContentSize({size.x, 200.0f});
	}
	updateMenuItemPositions();
}

float Theme::getTotalHeight() const {
	float totalHeight = 0;
	for (const auto &item : _menuItems) {
		totalHeight += item->getTotalHeight() + 5.0f; // Add spacing
	}
	return totalHeight;
}

void Theme::updateMenuItemPositions() {
	float currentY = _position.y;
	for (auto &item : _menuItems) {
		item->setPosition({_position.x, currentY});
		currentY += item->getTotalHeight() + 5.0f; // Add spacing between items
	}
}
