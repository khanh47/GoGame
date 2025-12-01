#pragma once
#include "raylib.h"
#include <functional>
#include <memory>
#include <string>

// Forward declaration
class MenuComponent;

class ExpandableMenuItemView {
private:
	Vector2 _position;
	Vector2 _size;
	Vector2 _expandedContentSize;
	bool _isHovered = false;
	bool _isExpanded = false;

	// Background colors
	Color _normalBackgroundColor = {251, 243, 213, 255};
	Color _hoverBackgroundColor = {214, 218, 200, 255};
	Color _expandedBackgroundColor = {230, 230, 230, 255};

	// Text colors
	Color _normalTextColor = {0, 0, 0, 255};
	Color _hoverTextColor = {255, 255, 255, 255};

	Font _font = GetFontDefault();
	int _fontSize = 24;
	float _fontSpacing = 1.0f;

	std::string _title;

	// Callback for rendering expanded content
	std::function<void(Vector2, Vector2)> _renderExpandedContent;

public:
	ExpandableMenuItemView(const Vector2 &position, const Vector2 &size, const std::string &title);

	bool isPointed(const Vector2 &point) const;
	bool isHeaderPointed(const Vector2 &point) const;
	Rectangle GetBoundingBox() const;
	Rectangle GetHeaderBoundingBox() const;

	void setHovered(bool hovered);
	bool getHovered() const;

	void setExpanded(bool expanded);
	bool isExpanded() const;
	void toggleExpanded();

	void render() const;

	void setPosition(Vector2 position) { _position = position; }
	void setSize(Vector2 size) { _size = size; }
	void setExpandedContentSize(Vector2 size) { _expandedContentSize = size; }

	const Vector2 &getPosition() const { return _position; }
	const Vector2 &getSize() const { return _size; }
	float getTotalHeight() const;

	void setTitle(const std::string &title) { _title = title; }
	const std::string &getTitle() const { return _title; }

	void setNormalBackgroundColor(Color color) { _normalBackgroundColor = color; }
	void setHoverBackgroundColor(Color color) { _hoverBackgroundColor = color; }
	void setExpandedBackgroundColor(Color color) { _expandedBackgroundColor = color; }

	void setNormalTextColor(Color color) { _normalTextColor = color; }
	void setHoverTextColor(Color color) { _hoverTextColor = color; }

	void setFont(Font font) { _font = font; }
	void setFontSize(int fontSize) { _fontSize = fontSize; }
	void setFontSpacing(float fontSpace) { _fontSpacing = fontSpace; }

	void setRenderExpandedContent(std::function<void(Vector2, Vector2)> callback) { _renderExpandedContent = callback; }
};
