#pragma once
#include "ICommand.h"
#include <memory>
#include <string>
#include <vector>

// Interface for menu components
class MenuComponent {
public:
	virtual ~MenuComponent() = default;
	virtual void addItem(std::shared_ptr<MenuComponent> component) = 0;
	virtual void removeItem(std::shared_ptr<MenuComponent> component) = 0;
	virtual MenuComponent *findItem(const std::string &title) = 0;

	// Setters for title and text
	virtual void setTitle(const std::string &title) = 0;
	virtual void setText(const std::string &text) = 0;
	virtual std::string getTitle() const = 0;
	virtual std::string getText() const = 0;

	// enable
	virtual void setEnabled(bool enabled) = 0;
	virtual bool isEnabled() const = 0;

	// command execution
	virtual void setCommand(std::unique_ptr<ICommand> command) = 0;
	virtual std::unique_ptr<ICommand> cloneCommand() const = 0;
	virtual const std::vector<std::shared_ptr<MenuComponent>> &getChildrens() const = 0;
};
