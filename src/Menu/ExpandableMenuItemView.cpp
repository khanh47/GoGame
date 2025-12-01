#include "ExpandableMenuItemView.h"
#include "ResourceManager.h"

ExpandableMenuItemView::ExpandableMenuItemView(const Vector2 &position, const Vector2 &size, const std::string &title)
		: _position(position), _size(size), _expandedContentSize({size.x, 100.0f}), _title(title) {
	_font = ResourceManager::getInstance().getFont("GozaruDemo");
}

bool ExpandableMenuItemView::isPointed(const Vector2 &point) const {
	Rectangle totalBounds = GetBoundingBox();
	return CheckCollisionPointRec(point, totalBounds);
}

bool ExpandableMenuItemView::isHeaderPointed(const Vector2 &point) const {
	Rectangle headerBounds = GetHeaderBoundingBox();
	return CheckCollisionPointRec(point, headerBounds);
}

Rectangle ExpandableMenuItemView::GetBoundingBox() const {
	float totalHeight = getTotalHeight();
	return {_position.x, _position.y, _size.x, totalHeight};
}

Rectangle ExpandableMenuItemView::GetHeaderBoundingBox() const { return {_position.x, _position.y, _size.x, _size.y}; }

void ExpandableMenuItemView::setHovered(bool hovered) { _isHovered = hovered; }

bool ExpandableMenuItemView::getHovered() const { return _isHovered; }

void ExpandableMenuItemView::setExpanded(bool expanded) { _isExpanded = expanded; }

bool ExpandableMenuItemView::isExpanded() const { return _isExpanded; }

void ExpandableMenuItemView::toggleExpanded() { _isExpanded = !_isExpanded; }

float ExpandableMenuItemView::getTotalHeight() const {
	if (_isExpanded) {
		return _size.y + _expandedContentSize.y;
	}
	return _size.y;
}

void ExpandableMenuItemView::render() const {
	// Draw header background
	Color bgColor = _isHovered ? _hoverBackgroundColor : _normalBackgroundColor;
	DrawRectangle((int)_position.x, (int)_position.y, (int)_size.x, (int)_size.y, bgColor);
	DrawRectangleLinesEx(GetHeaderBoundingBox(), 1.0f, DARKGRAY);

	// Draw title
	Color textColor = _isHovered ? _hoverTextColor : _normalTextColor;
	Vector2 textSize = MeasureTextEx(_font, _title.c_str(), (float)_fontSize, _fontSpacing);
	float textX = _position.x + 15.0f;
	float textY = _position.y + (_size.y - textSize.y) / 2.0f;
	DrawTextEx(_font, _title.c_str(), {textX, textY}, (float)_fontSize, _fontSpacing, textColor);

	// Draw expand/collapse indicator
	const char *indicator = _isExpanded ? "-" : "+";
	Vector2 indicatorSize = MeasureTextEx(_font, indicator, (float)_fontSize, _fontSpacing);
	float indicatorX = _position.x + _size.x - indicatorSize.x - 15.0f;
	float indicatorY = _position.y + (_size.y - indicatorSize.y) / 2.0f;
	DrawTextEx(_font, indicator, {indicatorX, indicatorY}, (float)_fontSize, _fontSpacing, textColor);

	// Draw expanded content if expanded
	if (_isExpanded) {
		Vector2 contentPos = {_position.x, _position.y + _size.y};
		DrawRectangle((int)contentPos.x, (int)contentPos.y, (int)_expandedContentSize.x, (int)_expandedContentSize.y,
									_expandedBackgroundColor);
		DrawRectangleLinesEx({contentPos.x, contentPos.y, _expandedContentSize.x, _expandedContentSize.y}, 1.0f, DARKGRAY);

		if (_renderExpandedContent) {
			_renderExpandedContent(contentPos, _expandedContentSize);
		}
	}
}
