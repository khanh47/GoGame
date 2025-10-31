#include "ListMenuView.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "MenuComponent.h"
#include <algorithm>
#include <cmath>
#include "MenuComponent.h"
#include "MenuView.h"

ListMenuView::ListMenuView(Rectangle area) : listArea(area) {
    updateScrollbarArea();
}

void ListMenuView::updateScrollbarArea() {
    scrollbarArea = {
        listArea.x + listArea.width - scrollbarWidth,
        listArea.y,
        scrollbarWidth,
        listArea.height
    };
}

void ListMenuView::calculateMaxScrollOffset() {
    float totalContentHeight = _itemViews.size() * (itemHeight + itemSpacing) - itemSpacing;
    maxScrollOffset = fmaxf(0.0f, totalContentHeight - listArea.height);
}

void ListMenuView::createNavigationItemViews(std::shared_ptr<MenuComponent> menuModel, GameState* gameState) {
    _itemViews.clear();

    if (gameState == nullptr) {
        for (size_t i = 0; i < menuModel->getChildrens().size(); ++i) {
            const auto& child = menuModel->getChildrens()[i];
            if (child) {
                Vector2 position = { 
                    listArea.x + 10.0f, 
                    listArea.y + i * (itemHeight + itemSpacing) 
                };
                Vector2 size = { listArea.width - scrollbarWidth - 20.0f, itemHeight };
                auto itemView = std::make_shared<MenuItemView>(position, size);
                itemView->setFont(ResourceManager::getInstance().getFont("GozaruDemo"));
                _itemViews.push_back(itemView);
            }
        }
    } else {
        _itemViews.clear();
        auto itemViews = gameState->createNavigationMenuButtonItemViews(menuModel);
        
        for (size_t i = 0; i < itemViews.size(); ++i) {
            if (itemViews[i]) {
                Vector2 position = { 
                    listArea.x + 10.0f, 
                    listArea.y + i * (itemHeight + itemSpacing) 
                };
                Vector2 size = { listArea.width - scrollbarWidth - 20.0f, itemHeight };
                itemViews[i]->setPosition(position);
                itemViews[i]->setSize(size);
                _itemViews.push_back(itemViews[i]);
            }
        }
    }
    
    calculateMaxScrollOffset();
    autoResizeToFitContent();
}

void ListMenuView::createInGameItemsViews(int numberOfItems) {
    _itemViews.clear();
    _itemViews.reserve(numberOfItems);
    
    for (int i = 0; i < numberOfItems; ++i) {
        Vector2 position = { 
            listArea.x + 10.0f, 
            listArea.y + i * (itemHeight + itemSpacing) 
        };
        Vector2 size = { listArea.width - scrollbarWidth - 20.0f, itemHeight };
        auto itemView = std::make_shared<MenuItemView>(position, size);
        itemView->setFont(ResourceManager::getInstance().getFont("GozaruDemo"));
        _itemViews.push_back(itemView);
    }
    
    calculateMaxScrollOffset();
    autoResizeToFitContent();
}

void ListMenuView::handleScrollInput() {
    Vector2 mousePos = GetMousePosition();
    
    // Handle mouse wheel scrolling
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0 && CheckCollisionPointRec(mousePos, listArea)) {
        scrollOffset -= wheelMove * 30.0f; // Scroll speed
        scrollOffset = fmaxf(0.0f, fminf(scrollOffset, maxScrollOffset));
    }
    
    // Handle scrollbar dragging
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // Check if clicked on scrollbar area (not just handle)
        if (CheckCollisionPointRec(mousePos, scrollbarArea)) {
            isDragging = true;
            // If clicked on track but not handle, jump to that position
            if (!isScrollbarHovered()) {
                float relativeY = mousePos.y - scrollbarArea.y;
                if (scrollbarArea.height > 0) {
                    float scrollRatio = relativeY / scrollbarArea.height;
                    scrollOffset = fmaxf(0.0f, fminf(scrollRatio * maxScrollOffset, maxScrollOffset));
                }
            }
        }
    }
    
    if (isDragging) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            float relativeY = mousePos.y - scrollbarArea.y;
            if (scrollbarArea.height > 0) {
                float scrollRatio = relativeY / scrollbarArea.height;
                scrollOffset = fmaxf(0.0f, fminf(scrollRatio * maxScrollOffset, maxScrollOffset));
            }
        } else {
            isDragging = false;
        }
    }
}

bool ListMenuView::isScrollbarHovered() const {
    Vector2 mousePos = GetMousePosition();
    Rectangle handleRect = {
        scrollbarArea.x,
        scrollbarArea.y + getScrollHandlePosition(),
        scrollbarArea.width,
        getScrollHandleHeight()
    };
    return CheckCollisionPointRec(mousePos, handleRect);
}

float ListMenuView::getScrollHandlePosition() const {
    if (maxScrollOffset <= 0) return 0.0f;
    float scrollRatio = scrollOffset / maxScrollOffset;
    return scrollRatio * (scrollbarArea.height - getScrollHandleHeight());
}

float ListMenuView::getScrollHandleHeight() const {
    if (maxScrollOffset <= 0) return scrollbarArea.height;
    float visibleRatio = listArea.height / (listArea.height + maxScrollOffset);
    return fmaxf(20.0f, visibleRatio * scrollbarArea.height);
}

void ListMenuView::updateScrollbar() {
    calculateMaxScrollOffset();
    
    // Clamp scroll offset to valid range
    scrollOffset = fmaxf(0.0f, fminf(scrollOffset, maxScrollOffset));
    
    // Update scrollbar area dimensions
    updateScrollbarArea();
}

Vector2 ListMenuView::getScrolledItemPosition(size_t index) const {
    if (index >= _itemViews.size() || !_itemViews[index]) {
        return {0, 0};
    }
    
    Vector2 originalPos = _itemViews[index]->getPosition();
    return {originalPos.x, originalPos.y - scrollOffset};
}

float ListMenuView::calculateRequiredContentHeight() const {
    if (_itemViews.empty()) {
        return 0.0f;
    }
    
    return _itemViews.size() * (itemHeight + itemSpacing) - itemSpacing;
}

void ListMenuView::autoResizeToFitContent() {
    float requiredHeight = calculateRequiredContentHeight();
    
    // Only resize if the required height is less than the current listArea height
    if (requiredHeight > 0 && requiredHeight < listArea.height) {
        // Maintain the same x, y, and width, but adjust the height
        listArea.height = requiredHeight;
        
        // Update related components
        updateScrollbarArea();
        calculateMaxScrollOffset();
        
        // Reset scroll offset since we no longer need scrolling
        scrollOffset = 0.0f;
    }
}

void ListMenuView::render(std::shared_ptr<MenuComponent> menuModel) const {
    const auto& menuItems = menuModel->getChildrens();
    
    // Draw background
    DrawRectangleRec(listArea, GRAY);
    DrawRectangleLinesEx(listArea, 2.0f, DARKGRAY);
    
    // Begin scissor mode for clipping
    BeginScissorMode((int)listArea.x, (int)listArea.y, (int)listArea.width - (int)scrollbarWidth, (int)listArea.height);
    
    // Draw menu items with scroll offset
    for (size_t i = 0; i < _itemViews.size() && i < menuItems.size(); ++i) {
        if (_itemViews[i] && menuItems[i]->isEnabled()) {
            // Calculate item position with scroll offset
            Vector2 originalPos = _itemViews[i]->getPosition();
            Vector2 scrolledPos = { originalPos.x, originalPos.y - scrollOffset };
            
            // Only draw items that are visible in the list area
            if (scrolledPos.y + itemHeight >= listArea.y && 
                scrolledPos.y <= listArea.y + listArea.height) {
                
                // Temporarily update position for drawing
                const_cast<MenuItemView*>(_itemViews[i].get())->setPosition(scrolledPos);
                _itemViews[i]->render(menuItems[i]);
                // Restore original position
                const_cast<MenuItemView*>(_itemViews[i].get())->setPosition(originalPos);
            }
        }
    }
    
    EndScissorMode();
    
    // Draw scrollbar if needed
    if (maxScrollOffset > 0) {
        // Draw scrollbar background
        DrawRectangleRec(scrollbarArea, scrollbarBackgroundColor);
        DrawRectangleLinesEx(scrollbarArea, 1.0f, GRAY);
        
        // Draw scrollbar handle
        Rectangle handleRect = {
            scrollbarArea.x,
            scrollbarArea.y + getScrollHandlePosition(),
            scrollbarArea.width,
            getScrollHandleHeight()
        };
        
        Color handleColor = isScrollbarHovered() ? scrollbarHandleHoverColor : scrollbarHandleColor;
        DrawRectangleRec(handleRect, handleColor);
        DrawRectangleLinesEx(handleRect, 1.0f, DARKGRAY);
    }
}