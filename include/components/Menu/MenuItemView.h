#pragma once
#include "raylib.h"
#include <memory>

// Forward declaration
class MenuComponent;

class MenuItemView {
private:
    Vector2 _position;
    Vector2 _size;
    bool _isHovered;
    bool _isSelected;

public:
    MenuItemView(const Vector2& position, const Vector2& size);

    bool isPointed(const Vector2& point) const;
    Rectangle GetBoundingBox() const;

    void setHovered(bool hovered);
    bool getHovered() const;

    void setSelected(bool selected);
    bool getSelected() const;

    void render(std::shared_ptr<MenuComponent> menuComponent) const;

    const Vector2& getPosition() const;
    const Vector2& getSize() const;
};
