#pragma once
#include "MenuComponent.h"
#include <memory>
#include <string>
#include <vector>

// Forward declaration
class ICommand;

class MenuItem : public MenuComponent {
private:
	std::string _title;
	std::string _text;
	bool _enabled;
	std::unique_ptr<ICommand> _command;

public:
	MenuItem(const std::string &title, bool enabled);
	void addItem(std::shared_ptr<MenuComponent> component) override;
	void removeItem(std::shared_ptr<MenuComponent> component) override;
	MenuComponent *findItem(const std::string &title) override;

	void setTitle(const std::string &title) override;
	void setText(const std::string &text) override;
	std::string getTitle() const override;
	std::string getText() const override;

	void setEnabled(bool enabled) override;
	bool isEnabled() const override;

	void setCommand(std::unique_ptr<ICommand> command) override;
	std::unique_ptr<ICommand> cloneCommand() const override;
	const std::vector<std::shared_ptr<MenuComponent>> &getChildrens() const override;
};