#pragma once
#include <memory>
#include <string>
#include "TextBoxMenuController.h"
#include "Text.h"

class InGameScene;

class TextBox {
public:
		TextBox(InGameScene* scene);
		~TextBox() = default;

		void open();
		void close();
		void closeAndCreate();

		bool isOpen() const { return _open; }

		void update(float dt);
		void render() const;
		void handleInput();

private:
		void init();

		InGameScene* _inGameScene = nullptr;
		bool _open = false;
		std::unique_ptr<TextBoxMenuController> _menuController;
    std::string _buffer;
    std::unique_ptr<Text> _text;
};
