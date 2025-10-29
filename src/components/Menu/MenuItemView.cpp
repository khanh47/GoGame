#include "MenuItemView.h"
#include "MenuComponent.h"

MenuItemView::MenuItemView(const Vector2& position, const Vector2& size)
    : _position(position), _size(size), _isHovered(false), _isSelected(false) {}

bool MenuItemView::isPointed(const Vector2& point) const {
    return CheckCollisionPointRec(point, GetBoundingBox());
}

Rectangle MenuItemView::GetBoundingBox() const {
    return { _position.x, _position.y, _size.x, _size.y };
}

void MenuItemView::setHovered(bool hovered) {
    _isHovered = hovered;
}

bool MenuItemView::getHovered() const {
    return _isHovered;
}

void MenuItemView::setSelected(bool selected) {
    _isSelected = selected;
}

bool MenuItemView::getSelected() const {
    return _isSelected;
}

void MenuItemView::render(std::shared_ptr<MenuComponent> menuComponent) const {
    // Render the menu item using the provided MenuComponent
}

const Vector2& MenuItemView::getPosition() const {
    return _position;
}

const Vector2& MenuItemView::getSize() const {
    return _size;
}