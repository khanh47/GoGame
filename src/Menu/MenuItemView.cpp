#include "MenuItemView.h"
#include "MenuComponent.h"

MenuItemView::MenuItemView(const Vector2 &position, const Vector2 &size)
		: _position(position), _size(size), _isHovered(false), _isSelected(false) {}

bool MenuItemView::isPointed(const Vector2 &point) const { return CheckCollisionPointRec(point, GetBoundingBox()); }

Rectangle MenuItemView::GetBoundingBox() const { return {_position.x, _position.y, _size.x, _size.y}; }

void MenuItemView::setHovered(bool hovered) { _isHovered = hovered; }

bool MenuItemView::getHovered() const { return _isHovered; }

void MenuItemView::setSelected(bool selected) { _isSelected = selected; }

bool MenuItemView::getSelected() const { return _isSelected; }

void MenuItemView::render(std::shared_ptr<MenuComponent> menuComponent) const {
	Color bgColor, textColor;

	if (!menuComponent->isEnabled()) {
		bgColor = disabledBackgroundColor;
		textColor = disabledTextColor;
	} else if (_isSelected) {
		bgColor = selectedBackgroundColor;
		textColor = selectedTextColor;
	} else if (_isHovered) {
		bgColor = hoverBackgroundColor;
		textColor = hoverTextColor;
	} else {
		bgColor = normalBackgroundColor;
		textColor = normalTextColor;
	}

	DrawRectangleV(_position, _size, bgColor);
	DrawRectangleLinesEx({_position.x, _position.y, _size.x, _size.y}, 2, BLACK);

	Vector2 textSize = MeasureTextEx(_font, menuComponent->getTitle().c_str(), _fontSize, _fontSpacing);
	Vector2 textPos = {_position.x + (_size.x - textSize.x) / 2, _position.y + (_size.y - textSize.y) / 2};
	DrawTextEx(_font, menuComponent->getTitle().c_str(), textPos, _fontSize, _fontSpacing, textColor);
}

const Vector2 &MenuItemView::getPosition() const { return _position; }

const Vector2 &MenuItemView::getSize() const { return _size; }