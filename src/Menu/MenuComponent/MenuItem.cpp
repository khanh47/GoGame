#include "MenuItem.h"
#include "MenuComponent.h"
#include "ICommand.h"

MenuItem::MenuItem(const std::string& title, bool enabled)
    : _title(title), _enabled(enabled), _command(nullptr) {}

void MenuItem::addItem(std::shared_ptr<MenuComponent> component) {}

void MenuItem::removeItem(std::shared_ptr<MenuComponent> component) {}

MenuComponent* MenuItem::findItem(const std::string& title)
{
    return nullptr; // MenuItem has no children
}

void MenuItem::setTitle(const std::string& title)
{
    _title = title;
}

void MenuItem::setText(const std::string& text)
{
    _text = text;
}

std::string MenuItem::getTitle() const
{
    return _title;
}

std::string MenuItem::getText() const
{
    return _text;
}

void MenuItem::setEnabled(bool enabled)
{
    _enabled = enabled;
}

bool MenuItem::isEnabled() const
{
    return _enabled;
}

void MenuItem::setCommand(std::unique_ptr<ICommand> command)
{
    _command = std::move(command);
}

std::unique_ptr<ICommand> MenuItem::cloneCommand() const
{
    if (_command) {
        return _command->clone();
    }
    return nullptr;
}

const std::vector<std::shared_ptr<MenuComponent>>& MenuItem::getChildrens() const
{
    static const std::vector<std::shared_ptr<MenuComponent>> emptyChildren;
    return emptyChildren; // MenuItem has no children
}