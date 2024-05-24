#include "ui.hpp"
#include "components.hpp"
#include "lua/lualoader.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "ui_util.hpp"

void UI::hotBar(Atlas& atlas, InventoryC& inventory, HotBarC& hotBar, int width, int height)
{

	float cellThickness = 1.0f;
	UIRowGridRec({0, (float)(GetScreenHeight() - height), width, height}, cellThickness, gridSpacing, hotBar.capacity, hotBar.curItem, RAYWHITE, Color{255, 255, 255, 32});

	UIRowGridIcon(atlas, inventory, {0, (float)(GetScreenHeight() - height), width, height}, 1.0f, gridSpacing, 0, hotBar.capacity);

	int xNumsOffset = 5;
	int yNumsOffset = 17;
	UIRowGridNumbers({xNumsOffset, (float)(GetScreenHeight() - height - yNumsOffset), width, height}, gridSpacing, hotBar.capacity);

	Rectangle bound = {0, (float)(GetScreenHeight() - height), (hotBar.capacity * width) + (gridSpacing * hotBar.capacity), hotBar.capacity * 48};
	bounds[HOTBAR] = bound;
	/* DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, PURPLE); */
}

void UI::miniMap(Rectangle mapSrc)
{
	UIWindowOutline(mapSrc);
	bounds[MINIMAP] = mapSrc;
}

void UI::inventory(Atlas& atlas, InventoryC& inventory, HotBarC& hotBar, int width, int height, int cellsPerRow)
{
	Rectangle hotBarRec = bounds[HOTBAR];

	Rectangle windowRec = Rectangle{hotBarRec.x, hotBarRec.y - 300, width, height};
	/*UIWindowOutline(windowRec);*/
	for (int i = 0; i < (int)(inventory.slots.size() / cellsPerRow); i++) {
		UIRowGridRec({0, windowRec.y - (i * 48), 48, 48}, 1.0f, gridSpacing, cellsPerRow, -1, RAYWHITE, Color{255, 255, 255, 32});
		UIRowGridIcon(atlas, inventory, {0, windowRec.y - (i * 48), 48, 48}, 1.0f, gridSpacing, i * cellsPerRow, cellsPerRow);
	}
}

bool UI::mouseOutOfBounds()
{
	for (int i = 0; i < 2; i++) {
		if (CheckCollisionPointRec(GetMousePosition(), bounds[i])) {
			return false;
		}
	}
	return true;
}
