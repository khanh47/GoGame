#pragma once
#include "ThemeController.h"

class BoardController : public ThemeController {
public:
	BoardController();
	~BoardController() override = default;

protected:
	void applyTheme() override;
	void applyColor() override;
	void syncFromSettings() override;
};
