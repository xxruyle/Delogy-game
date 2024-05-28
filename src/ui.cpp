#include "ui.hpp"
#include "components.hpp"
#include "item_data.hpp"
#include "lua/lualoader.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "ui_util.hpp"
#include <cstddef>

UI::UI(SpriteDrawSystem* spriteDrawSystem) { drawSystem = spriteDrawSystem; }

void UI::hotBar(InventoryC& inventory, HotBarC& hotBar, Vector2 src, int width, int height)
{

	float cellThickness = 1.0f;
	int selection = UIRowGridRec({src.x, src.y, width, height}, cellThickness, gridSpacing, hotBar.capacity, hotBar.curItem, RAYWHITE, Color{255, 255, 255, 32});
	Rectangle iconSrc = {src.x, src.y, width, height};
	inventoryRowSlots(inventory, 0, hotBar.capacity, iconSrc);

	int xNumsOffset = 5;
	int yNumsOffset = 17;
	UIRowGridNumbers({src.x, src.y - 18, width, height}, gridSpacing, hotBar.capacity);

	Rectangle bound = {0, (float)(GetScreenHeight() - height), (hotBar.capacity * width) + (gridSpacing * hotBar.capacity), hotBar.capacity * 48};
	bounds[HOTBAR] = bound;

	bool cellHovered = false;
	if (selection != -1) {
		cellHovered = true;
	}

	checkDragAndDrop(selection, selection); // updates dragAndDropper
	checkResetDrop(cellHovered);			// reset if invalid drop

	if (dragAndDrop.holdActive) {
		Vector2 mousePos = GetMousePosition();
		Rectangle itemRec = {mousePos.x - 16, mousePos.y - 16, 40, 40};
		UIIcon(drawSystem->smallAtlas, dragAndDrop.iconAtlasSrc, itemRec);
	}

	handleDropEvent(inventory); // handle behavior of a valid drop event
}

void UI::miniMap(Rectangle mapSrc)
{
	UIWindowOutline(mapSrc);
	bounds[MINIMAP] = mapSrc;
}

void UI::inventoryRowSlots(InventoryC& inventory, int curRow, int numCells, Rectangle rowCellSrc)
{
	int iconSize = 40;
	rowCellSrc.x -= rowCellSrc.width;
	/*std::cout << numCells << std::endl;*/
	for (int j = 0; j < numCells; j++) {
		int inventoryIndex = j + (curRow * numCells);
		rowCellSrc.x += rowCellSrc.width + gridSpacing;
		if (inventory.slots[inventoryIndex] != NULL_ITEM) {
			int itemId = inventory.slots[inventoryIndex];
			Item curItem = itemids[itemId];
			Rectangle itemTexture = {(float)curItem.x, (float)curItem.y, 16, 16};

			// if drag and dropper is active we just set the icon texture atlas src for it
			if (dragAndDrop.holdActive && (inventoryIndex == dragAndDrop.holdedSlotIndex)) {
				dragAndDrop.iconAtlasSrc = itemTexture;
			}
			else {
				Rectangle itemRec = {rowCellSrc.x + 4, rowCellSrc.y + 4, iconSize, iconSize};
				UIIcon(drawSystem->smallAtlas, itemTexture, itemRec);
				DrawText(std::to_string(inventory.stacks[inventoryIndex]).c_str(), itemRec.x + 5, itemRec.y + itemRec.height - 15, 20, RAYWHITE);
			}
		}
	}
}

void UI::inventory(InventoryC& inventory, Vector2 src, int width, int height, int cellsPerRow)
{
	int iconSize = 40;

	/*UIWindowOutline(windowRec);*/

	// lower rows in the inventory are drawn first
	int numRows = (int)(inventory.slots.size() / cellsPerRow);

	bool cellHovered = false;
	for (int row = 0; row < numRows; row++) {
		Rectangle iconSrc = {src.x, src.y - (row * height), width, height};
		int selection = UIRowGridRec({src.x, src.y - (row * height), width, height}, 1.0f, gridSpacing, cellsPerRow, -1, RAYWHITE, Color{255, 255, 255, 32});

		// if at least one cell was hovered over, set to true
		if (selection != -1) {
			cellHovered = true;
		}

		int inventoryIndex = selection + (row * cellsPerRow);
		checkDragAndDrop(inventoryIndex, selection); // updates dragAndDropper

		inventoryRowSlots(inventory, row, cellsPerRow, iconSrc);
	}

	checkResetDrop(cellHovered); // reset if invalid drop

	if (dragAndDrop.holdActive) {
		Vector2 mousePos = GetMousePosition();
		Rectangle itemRec = {mousePos.x - 16, mousePos.y - 16, 40, 40};
		UIIcon(drawSystem->smallAtlas, dragAndDrop.iconAtlasSrc, itemRec);
	}

	handleDropEvent(inventory); // handle behavior of a valid drop event
}

void UI::checkResetDrop(bool cellFound)
{
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !cellFound) {
		dragAndDrop = {0, 0, Rectangle{0, 0, 0, 0}, false, false};
	}
}

void UI::checkDragAndDrop(int inventoryIndex, int gridValidity)
{
	if (gridValidity != -1) {
		// holding icon
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !dragAndDrop.holdActive) {
			dragAndDrop.holdActive = true;
			dragAndDrop.holdedSlotIndex = inventoryIndex;
		}

		// dropping icon
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && dragAndDrop.holdActive) {
			dragAndDrop.droppedSlotIndex = inventoryIndex;
			dragAndDrop.dropActive = true;
			dragAndDrop.holdActive = false;
		}
	}
}

void UI::handleDropEvent(InventoryC& inventory)
{
	if (dragAndDrop.dropActive) {
		int slotIndexDroppedBy = dragAndDrop.holdedSlotIndex;
		int slotIndexDroppedOn = dragAndDrop.droppedSlotIndex;

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
		dragAndDrop = {0, 0, Rectangle{0, 0, 0, 0}, false, false}; // reset dragAndDrop
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
