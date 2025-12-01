#pragma once
#include "Text.h"
#include "TextBoxMenuController.h"
#include <memory>
#include <string>

class InGameScene;
class DataManager;

class TextBox {
public:
	TextBox(InGameScene *scene, DataManager *data);
	~TextBox() = default;

	void open();
	void close();
	void closeAndCreate();

	bool isOpen() const { return _open; }

	void update();
	void render() const;
	void handleInput();

private:
	void init();

	InGameScene *_inGameScene = nullptr;
	DataManager *_dataManager = nullptr;
	bool _open = false;
	std::unique_ptr<TextBoxMenuController> _menuController;
	std::string _buffer;
	std::unique_ptr<Text> _text;
};
