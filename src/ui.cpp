#include "ui.hpp"
#include "components.hpp"
#include "item_data.hpp"
#include "lua/lualoader.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "ui_util.hpp"
#include <cstddef>

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
	// lower rows in the inventory are drawn first
	for (int i = 0; i < (int)(inventory.slots.size() / cellsPerRow); i++) {
		UIRowGridRec({0, windowRec.y - (i * 48), 48, 48}, 1.0f, gridSpacing, cellsPerRow, -1, RAYWHITE, Color{255, 255, 255, 32});
		UIRowGridIcon(atlas, inventory, {0, windowRec.y - (i * 48), 48, 48}, 1.0f, gridSpacing, i * cellsPerRow, cellsPerRow);
	}

	if (UIEvent::dropped.active) {
		int slotIndexDroppedBy = UIEvent::dropped.holdedSlotIndex;
		int slotIndexDroppedOn = UIEvent::dropped.slotIndex;

		/*std::cout << "Dropped By: " << slotIndexDroppedBy << std::endl;*/
		/*std::cout << "Dropped On: " << slotIndexDroppedOn << std::endl << std::endl;*/

		if ((slotIndexDroppedOn != slotIndexDroppedBy)) {
			if ((inventory.slots[slotIndexDroppedOn] == NULL_ITEM)) { // if slot dropped on is null
				inventory.slots[slotIndexDroppedOn] = inventory.slots[slotIndexDroppedBy];
				inventory.stacks[slotIndexDroppedOn] = inventory.stacks[slotIndexDroppedBy];

				inventory.slots[slotIndexDroppedBy] = NULL_ITEM;
				inventory.slots[slotIndexDroppedBy] = 0;
			}
			// if slot dropped on equals the item
			else if (inventory.slots[slotIndexDroppedOn] == inventory.slots[slotIndexDroppedBy]) {
				inventory.stacks[slotIndexDroppedOn] += inventory.stacks[slotIndexDroppedBy];
				inventory.slots[slotIndexDroppedBy] = NULL_ITEM;
				inventory.stacks[slotIndexDroppedBy] = 0;
			}
		}
		UIEvent::dropped.active = false;
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
