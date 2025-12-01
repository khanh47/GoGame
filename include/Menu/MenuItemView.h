#pragma once
#include "raylib.h"
#include <memory>

// Forward declaration
class MenuComponent;

class MenuItemView {
private:
	Vector2 _position;
	Vector2 _size;
	bool _isHovered = false;
	bool _isSelected = false;

	// Background colors
	Color normalBackgroundColor = {251, 243, 213, 255};		// rgb(251, 243, 213)
	Color hoverBackgroundColor = {214, 218, 200, 255};		// rgb(214, 218, 200)
	Color selectedBackgroundColor = {100, 149, 237, 255}; // Cornflower blue for selected
	Color disabledBackgroundColor = {156, 175, 170, 255}; // rgb(156, 175, 170)

	// Text colors
	Color normalTextColor = {0, 0, 0, 255};					// BLACK
	Color hoverTextColor = {255, 255, 255, 255};		// WHITE
	Color selectedTextColor = {255, 255, 255, 255}; // WHITE for selected
	Color disabledTextColor = {169, 169, 169, 255}; // DARKGRAY

	Font _font = GetFontDefault();
	int _fontSize = 30;
	float _fontSpacing = 1.0f;

public:
	MenuItemView(const Vector2 &position, const Vector2 &size);

	bool isPointed(const Vector2 &point) const;
	Rectangle GetBoundingBox() const;

	void setHovered(bool hovered);
	bool getHovered() const;

	void setSelected(bool selected);
	bool getSelected() const;

	void render(std::shared_ptr<MenuComponent> menuComponent) const;

	void setPosition(Vector2 position) { _position = position; }
	void setSize(Vector2 size) { _size = size; }

	const Vector2 &getPosition() const;
	const Vector2 &getSize() const;

	void setNormalBackgroundColor(Color color) { normalBackgroundColor = color; }
	void setHoverBackgroundColor(Color color) { hoverBackgroundColor = color; }
	void setSelectedBackgroundColor(Color color) { selectedBackgroundColor = color; }
	void setDisabledBackgroundColor(Color color) { disabledBackgroundColor = color; }

	void setNormalTextColor(Color color) { normalTextColor = color; }
	void setHoverTextColor(Color color) { hoverTextColor = color; }
	void setSelectedTextColor(Color color) { selectedTextColor = color; }
	void setDisabledTextColor(Color color) { disabledTextColor = color; }

	void setFont(Font font) { _font = font; }
	void setFontSize(int fontSize) { _fontSize = fontSize; }
	void setFontSpacing(float fontSpace) { _fontSpacing = fontSpace; }
};
