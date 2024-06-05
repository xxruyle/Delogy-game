#include "ui.hpp"
#include "components.hpp"
#include "entt/entity/entity.hpp"
#include "item_data.hpp"
#include "lua/lualoader.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "ui_util.hpp"

UI::UI(SpriteDrawSystem* spriteDrawSystem) { drawSystem = spriteDrawSystem; }

void UI::hotBar(InventoryC& inventory, HotBarC& hotBar, Vector2 src, int width, int height)
{

	float cellThickness = 1.0f;
	int selection = UIRowGridRec({src.x, src.y, width, height}, cellThickness, gridSpacing, hotBar.capacity, hotBar.curItem, RAYWHITE, Color{255, 255, 255, 32});
	Rectangle iconSrc = {src.x, src.y, width, height};
	inventoryRowSlots(inventory, 0, width, hotBar.capacity, iconSrc);

	int xNumsOffset = 5;
	int yNumsOffset = 17;
	UIRowGridNumbers({src.x, src.y - 18, width, height}, gridSpacing, hotBar.capacity);

	Rectangle bound = {0, (float)(GetScreenHeight() - height), (hotBar.capacity * width) + (gridSpacing * hotBar.capacity), hotBar.capacity * 48};
	bounds[HOTBAR] = bound;

	bool cellHovered = false;
	if (selection != -1) {
		cellHovered = true;
	}

	checkDragAndDrop(inventory, selection, selection); // updates dragAndDropper
	checkResetDrop(cellHovered);					   // reset if invalid drop

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

void UI::inventoryRowSlots(InventoryC& inventory, int curRow, int iconSize, int numCells, Rectangle rowCellSrc)
{
	rowCellSrc.x -= rowCellSrc.width;
	for (int j = 0; j < numCells; j++) {
		int inventoryIndex = j + (curRow * numCells);
		rowCellSrc.x += rowCellSrc.width + gridSpacing;
		if (inventory.slots[inventoryIndex] != NULL_ITEM) {
			int itemId = inventory.slots[inventoryIndex];
			Item curItem = itemids[itemId];
			Rectangle itemTexture = {(float)curItem.x, (float)curItem.y, 16, 16};

			// if drag and dropper is active we just set the icon texture atlas src for it
			if (dragAndDrop.holdActive && (inventoryIndex == dragAndDrop.holdedSlotIndex) && &inventory == dragAndDrop.inventoryHeld) {
				dragAndDrop.iconAtlasSrc = itemTexture;
			}
			else {
				int centeredOffset = (iconSize / 10);
				Rectangle itemRec = {rowCellSrc.x + centeredOffset, rowCellSrc.y + centeredOffset, iconSize - (centeredOffset * 2), iconSize - (centeredOffset * 2)};
				UIIcon(drawSystem->smallAtlas, itemTexture, itemRec);
				DrawText(std::to_string(inventory.stacks[inventoryIndex]).c_str(), itemRec.x + 5, itemRec.y + itemRec.height - 15, 20, RAYWHITE);
			}
		}
	}
}

void UI::inventory(InventoryC& inventory, Vector2 src, int width, int height, int cellsPerRow)
{
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
		checkDragAndDrop(inventory, inventoryIndex, selection); // updates dragAndDropper

		inventoryRowSlots(inventory, row, width, cellsPerRow, iconSrc);
	}

	/*checkResetDrop(cellHovered); // reset if invalid drop*/

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
		dragAndDrop = {nullptr, nullptr, 0, 0, Rectangle{0, 0, 0, 0}, false, false};
	}
}

void UI::checkDragAndDrop(InventoryC& inventory, int inventoryIndex, int gridValidity)
{
	if (gridValidity != -1) {
		// holding icon
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !dragAndDrop.holdActive) {
			dragAndDrop.holdActive = true;
			dragAndDrop.holdedSlotIndex = inventoryIndex;
			dragAndDrop.inventoryHeld = &inventory;
		}

		// dropping icon
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && dragAndDrop.holdActive) {
			dragAndDrop.droppedSlotIndex = inventoryIndex;
			dragAndDrop.dropActive = true;
			dragAndDrop.inventoryDropped = &inventory;
			dragAndDrop.holdActive = false;
		}
	}
}

void UI::handleDropEvent(InventoryC& inventory)
{
	if (dragAndDrop.dropActive && dragAndDrop.inventoryHeld != nullptr && dragAndDrop.inventoryDropped != nullptr) {
		int slotIndexDroppedBy = dragAndDrop.holdedSlotIndex;
		int slotIndexDroppedOn = dragAndDrop.droppedSlotIndex;

		if ((dragAndDrop.inventoryHeld == dragAndDrop.inventoryDropped) && (slotIndexDroppedBy == slotIndexDroppedOn)) {
			// change nothing
			/*std::cout << "same inventory" << std::endl;*/
			/*dragAndDrop.inventoryDropped->slots[slotIndexDroppedOn] = dragAndDrop.inventoryHeld->slots[slotIndexDroppedBy];*/
			/*dragAndDrop.inventoryDropped->stacks[slotIndexDroppedOn] = dragAndDrop.inventoryHeld->stacks[slotIndexDroppedBy];*/
		}
		else if ((dragAndDrop.inventoryDropped->slots[slotIndexDroppedOn] == NULL_ITEM)) { // if slot dropped on is null
			/*std::cout << "null drop" << std::endl;*/
			dragAndDrop.inventoryDropped->slots[slotIndexDroppedOn] = dragAndDrop.inventoryHeld->slots[slotIndexDroppedBy];
			dragAndDrop.inventoryDropped->stacks[slotIndexDroppedOn] = dragAndDrop.inventoryHeld->stacks[slotIndexDroppedBy];
			dragAndDrop.inventoryHeld->slots[slotIndexDroppedBy] = NULL_ITEM;
			dragAndDrop.inventoryHeld->stacks[slotIndexDroppedBy] = 0;
		}
		// if slot dropped on equals the item
		else if (dragAndDrop.inventoryDropped->slots[slotIndexDroppedOn] == dragAndDrop.inventoryHeld->slots[slotIndexDroppedBy]) {
			/*std::cout << "stack" << std::endl;*/
			dragAndDrop.inventoryDropped->stacks[slotIndexDroppedOn] += dragAndDrop.inventoryHeld->stacks[slotIndexDroppedBy];
			dragAndDrop.inventoryHeld->slots[slotIndexDroppedBy] = NULL_ITEM;
			dragAndDrop.inventoryHeld->stacks[slotIndexDroppedBy] = 0;
		}

		dragAndDrop = {nullptr, nullptr, 0, 0, Rectangle{0, 0, 0, 0}, false, false}; // reset dragAndDrop
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
