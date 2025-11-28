#pragma once
#include <string>
#include <memory>
#include <vector>
#include "MenuComponent.h"

// Forward declaration
class ICommand;

class Menu : public MenuComponent {
private:
    std::string _title;
    std::string _text;
    bool _enabled;
    std::unique_ptr<ICommand> _command;
    std::vector<std::shared_ptr<MenuComponent>> _children;

public:
    Menu(const std::string& title, bool enabled);
    void addItem(std::shared_ptr<MenuComponent> component) override;
    void removeItem(std::shared_ptr<MenuComponent> component) override;
    MenuComponent* findItem(const std::string& title) override;

    void setTitle(const std::string& title) override;
    void setText(const std::string& text) override;
    std::string getTitle() const override;
    std::string getText() const override;

    void setEnabled(bool enabled) override;
    bool isEnabled() const override;

    void setCommand(std::unique_ptr<ICommand> command) override;
    std::unique_ptr<ICommand> cloneCommand() const override;
    const std::vector<std::shared_ptr<MenuComponent>>& getChildrens() const override;
};