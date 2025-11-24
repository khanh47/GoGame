#include "Text.h"
#include "ResourceManager.h"

Text::Text(Rectangle area)
    : _area(area) {}

void Text::setDependencies(const Rectangle& area) {
		_area = area;
}

void Text::setText(const std::string& text) {
    _text = text;
}

const std::string& Text::getText() const {
    return _text;
}

void Text::render() const {
    DrawRectangleRec(_area, WHITE);
    DrawRectangleLinesEx(_area, 3, BLACK);
		Font font = ResourceManager::getInstance().getFont("GozaruDemo");

		int textLen = (int) _text.size() * 10;
    DrawTextEx(
				font,
				_text.c_str(),
				{_area.x + (_area.width - textLen) / 2.0f,
        _area.y + 10},
        28,
				3,
        BLACK);
}

