#pragma once
#include "MenuItemView.h"
#include "MenuView.h"
#include <memory>

// Forward declaration
class MenuComponent;

class ListMenuView : public IMenuView {
private:
	float scrollOffset = 0.0f;
	float itemHeight = 40.0f;
	float itemSpacing = 5.0f;
	float scrollbarWidth = 15.0f;
	Rectangle listArea;
	Rectangle scrollbarArea;
	bool isDragging = false;
	float maxScrollOffset = 0.0f;

	// Scrollbar styling
	Color scrollbarBackgroundColor = {200, 200, 200, 255};
	Color scrollbarHandleColor = {120, 120, 120, 255};
	Color scrollbarHandleHoverColor = {80, 80, 80, 255};

public:
	ListMenuView(Rectangle area = {50, 100, 300, 400});

	void createNavigationItemViews(std::shared_ptr<MenuComponent> menuModel, GameState *gameState) override;
	void createInGameItemsViews(int numberOfItems) override;
	void createSavedGameItemsViews(int numberOfItems) override;
	void createSettingsItemsViews(int numberOfItems) override;
	void createPassButton(int numberOfItems) override {};
	void render(std::shared_ptr<MenuComponent> menuModel) const override;

	// Scrolling methods
	void handleScrollInput();
	void updateScrollbar();
	bool isScrollbarHovered() const;
	float getScrollHandlePosition() const;
	float getScrollHandleHeight() const;

	// Get item position accounting for scroll offset
	Vector2 getScrolledItemPosition(size_t index) const;

	// Get the list area rectangle
	Rectangle getListArea() const { return listArea; }

	// Auto-resize functionality
	void autoResizeToFitContent();
	float calculateRequiredContentHeight() const;

	// Setters for customization
	void setListArea(Rectangle area) {
		listArea = area;
		updateScrollbarArea();
	}
	void setItemHeight(float height) { itemHeight = height; }
	void setItemSpacing(float spacing) { itemSpacing = spacing; }
	void setScrollbarWidth(float width) {
		scrollbarWidth = width;
		updateScrollbarArea();
	}

private:
	void updateScrollbarArea();
	void calculateMaxScrollOffset();
};
