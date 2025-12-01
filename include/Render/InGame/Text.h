#pragma once
#include <raylib.h>
#include <string>

class Text {
public:
	Text(Rectangle area);

	void setDependencies(const Rectangle &area);
	void render() const;
	void setText(const std::string &text);
	const std::string &getText() const;

private:
	Rectangle _area;
	std::string _text;
};
