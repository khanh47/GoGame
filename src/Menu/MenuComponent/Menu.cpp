#include "Menu.h"
#include "ICommand.h"
#include <algorithm>

Menu::Menu(const std::string& title, bool enabled)
    : _title(title), _enabled(enabled), _command(nullptr) {}

void Menu::addItem(std::shared_ptr<MenuComponent> component)
{
    _children.push_back(component);
}

void Menu::removeItem(std::shared_ptr<MenuComponent> component)
{
    _children.erase(std::remove(_children.begin(), _children.end(), component), _children.end());
}

MenuComponent *Menu::findItem(const std::string &title)
{
    if (_title == title) {
        return this;
    }

    for (const auto& component : _children) {
        if (component->getTitle() == title) {
            return component.get();
        }
        MenuComponent* found = component->findItem(title);
        if (found) {
            return found;
        }
    }
    return nullptr;
}

void Menu::setTitle(const std::string &title)
{
    _title = title;
}

void Menu::setText(const std::string &text)
{
    _text = text;
}

std::string Menu::getTitle() const
{
    return _title;
}

std::string Menu::getText() const
{
    return _text;
}

void Menu::setEnabled(bool enabled)
{
    _enabled = enabled;
}

bool Menu::isEnabled() const
{
    return _enabled;
}

void Menu::setCommand(std::unique_ptr<ICommand> command)
{
    _command = std::move(command);

}

std::unique_ptr<ICommand> Menu::cloneCommand() const
{
    if (_command) {
        return _command->clone(); // Assumes ICommand has a clone method
    }
    return nullptr;
}

const std::vector<std::shared_ptr<MenuComponent>> &Menu::getChildrens() const
{
    return _children;
}
