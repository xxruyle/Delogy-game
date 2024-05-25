#include "ui_util.hpp"
#include "input_system.hpp"
#include "item_data.hpp"
#include "raylib.h"
#include <string>

namespace UIEvent {
UIHoldIcon holded = {0, Rectangle{0, 0, 0, 0}, Vector2{0, 0}, false};
UIDropIcon dropped = {0, 0, false};
}; // namespace UIEvent

// pass -1 if you do not want a cell selected by default
int UIRowGridRec(Rectangle cellSrc, float thickness, float spacing, int numCells, int cellSelected, Color color, Color backgroundColor)
{
	int result = -1; // if the mouse is not in any cell
	Vector2 mousePos = GetMousePosition();
	Color tempBackground = backgroundColor;

	cellSrc.x -= cellSrc.width; // re-orient cellSrc to prepare for additions in the loop
	for (int i = 0; i < numCells; i++) {
		cellSrc.x += cellSrc.width + spacing; // incrememnt the x src each iteration
		bool mouseIn = CheckCollisionPointRec(mousePos, cellSrc);
		if (mouseIn) { // if the mouse is in one of the boxes change the result to that box's index
			result = i;
		}

		DrawRectangleLinesEx(cellSrc, thickness, WHITE); // Drawing the outline

		if (GRID_HIGHLIGHT) {
			if (result == i) { // if the result is the current box change the alpha value
				tempBackground.a = 100;
			}
			else if (i == cellSelected) { // if the result is the current cell selected change the alpha value
				tempBackground.a = 110;
			}
			else {
				tempBackground.a = 32;
			}
		}
		DrawRectangleRec(cellSrc, tempBackground); // Drawing the background
	}
	return result; // returning the cell a mouse position was in
}

void UIRowGridIcon(Atlas& atlas, InventoryC& playerInventory, Rectangle cellSrc, float thickness, float spacing, int startIndexSlot, int numCells)
{
	// TODO: This needs to be less dependent on inventory or just put it in the ui.inventory() function and make a UICell function()?
	if (numCells + startIndexSlot <= playerInventory.slots.size()) {
		int iconSize = 40;
		Vector2 mousePos = GetMousePosition();
		cellSrc.x -= cellSrc.width; // re-orient cellSrc to prepare for additions in the loop
		for (int i = 0; i < numCells; i++) {
			cellSrc.x += cellSrc.width + spacing; // incrememnt the x src each iteration
			if (UIEvent::holded.active && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				if (CheckCollisionPointRec(mousePos, cellSrc)) {
					// instantiate the dropped event
					UIEvent::dropped = {UIEvent::holded.slotIndex, i + startIndexSlot, true};

					// deactivate holded event
					UIEvent::holded = {0, Rectangle{0, 0, 0, 0}, Vector2{0, 0}, false};
				}
			}

			if (playerInventory.slots[i + startIndexSlot] != NULL_ITEM) {

				int itemId = playerInventory.slots[i + startIndexSlot];
				Item curItem = itemids[itemId];
				Rectangle itemTexture = {(float)curItem.x, (float)curItem.y, 16, 16};
				Rectangle itemRec = {cellSrc.x + 4, cellSrc.y + 4, iconSize, iconSize};

				// create a UI Hold Event if the user holds an icon
				if (!UIEvent::holded.active && CheckCollisionPointRec(mousePos, cellSrc)) {
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
						UIEvent::holded = {i + startIndexSlot, itemTexture, mousePos, true};
					}
				}

				// TODO This sucks because it's too much for one function
				// preparing for drag and drop implementation
				if (UIEvent::holded.active && (UIEvent::holded.slotIndex == (i + startIndexSlot))) {
					continue;
				}
				else {
					DrawTexturePro(atlas.texture, itemTexture, itemRec, {0, 0}, 0.0f, WHITE);
					DrawText(std::to_string(playerInventory.stacks[i + startIndexSlot]).c_str(), itemRec.x + 5, itemRec.y + itemRec.height - 15, 20, RAYWHITE);
				}
			}
		}

		// draw the icon being held by the user
		if (UIEvent::holded.active) {
			DrawTexturePro(atlas.texture, UIEvent::holded.iconAtlasSrc, Rectangle{mousePos.x - 16, mousePos.y - 16, 40, 40}, {0, 0}, 0.0f, WHITE);
		}
	}
}

void UIRowGridNumbers(Rectangle cellSrc, float spacing, int numCells)
{

	cellSrc.x -= cellSrc.width; // re-orient cellSrc to prepare for additions in the loop
	for (int i = 0; i < numCells; i++) {
		cellSrc.x += cellSrc.width + spacing; // incrememnt the x src each iteration
		DrawText(std::to_string(i + 1).c_str(), cellSrc.x, cellSrc.y, 18, WHITE);
	}
}

void UIWindowOutline(Rectangle cellSrc)
{
	/* DrawRectangleGradientEx(cellSrc, BLACK, GRAY, GREEN, PURPLE); */
	DrawRectangleRoundedLines(cellSrc, 0.01f, 1, 2.0f, WHITE);
}
