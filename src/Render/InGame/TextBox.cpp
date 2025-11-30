#include "TextBox.h"
#include "ButtonMenuView.h"
#include "DataManager.h"
#include "InGameScene.h"
#include "Text.h"
#include "colors.h"
#include "raylib.h"
#include <iostream>

TextBox::TextBox(InGameScene* scene)
    : _inGameScene(scene) {}

void TextBox::open() {
    if (_open) return;
    _open = true;
		init();
    _buffer.clear();
		Rectangle box = { (GetScreenWidth() - 200) / 2.0f, (GetScreenHeight() - 50) / 2.0f, 200, 50 };
		_text = std::make_unique<Text>(box);
    _text->setText("");
}

void TextBox::closeAndCreate() {
	std::cout << _buffer << std::endl;
	if (createNewSaveFile(_buffer))
		std::cout << "Create new saved file success\n";
	else
		std::cout << "Create new saved file failed\n";
	close();
}

void TextBox::close() {
  _open = false;
	_menuController.reset();
  _buffer.clear();
}

void TextBox::init() {
  _menuController = std::make_unique<TextBoxMenuController>(_inGameScene);
  _menuController->setViewStrategy(std::make_unique<ButtonMenuView>());
  _menuController->createMenu();
}

void TextBox::update(float dt) {
    if (!_open || !_menuController) return;
    _menuController->update();

    int key = GetCharPressed();

    // Capture characters
    while (key > 0) {
        if (key >= 32 && key <= 125) {
            _buffer.push_back((char)key);
            _text->setText(_buffer);
        }
        key = GetCharPressed();
    }

    // Backspace
    if (IsKeyPressed(KEY_BACKSPACE) && !_buffer.empty()) {
        _buffer.pop_back();
        _text->setText(_buffer);
    }
		float lengthBox = _buffer.size() * 20 + 200;
		Rectangle box = { (GetScreenWidth() - lengthBox) / 2.0f, (GetScreenHeight() - 50) / 2.0f, lengthBox, 50 };
		_text->setDependencies(box);
}

void TextBox::render() const {
    if (!_open || !_menuController) return;

		int screenWidth = GetScreenWidth();
		int screenHeight = GetScreenHeight();
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK,0.6f));
		Rectangle box = { (screenWidth - 800) / 2.0f, (screenHeight - 600) / 2.0f, 800, 600 };
		DrawRectangleRounded(box, 0.2f, 16, overlay);

		_menuController->render();
		_text->render();
}

void TextBox::handleInput() {
    if (!_open || !_menuController) return;
    _menuController->handleInput();
}

