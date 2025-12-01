#pragma once
#include "ThemeController.h"

class StoneController : public ThemeController {
public:
    StoneController();
    ~StoneController() override = default;

protected:
    void applyTheme() override;
    void applyColor() override;
    void syncFromSettings() override;
};
