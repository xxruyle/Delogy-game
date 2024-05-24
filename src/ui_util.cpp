#include "ui_util.hpp"
#include "item_data.hpp"
#include "raylib.h"
#include <string>

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
	if (numCells + startIndexSlot <= playerInventory.slots.size()) {
		cellSrc.x -= cellSrc.width; // re-orient cellSrc to prepare for additions in the loop
		for (int i = 0; i < numCells; i++) {
			if (playerInventory.slots[i + startIndexSlot] != NULL_ITEM) {
				cellSrc.x += cellSrc.width + spacing; // incrememnt the x src each iteration

				int itemId = playerInventory.slots[i + startIndexSlot];
				Item curItem = itemids[itemId];
				Rectangle itemTexture = {(float)curItem.x, (float)curItem.y, 16, 16};
				Rectangle itemRec = {cellSrc.x + 4, cellSrc.y + 4, 40, 40};
				Vector2 mousePos = GetMousePosition();
				// preparing for drag and drop implementation
				/*if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), cellSrc)) {*/
				/*	DrawTexturePro(atlas.texture, itemTexture, Rectangle{mousePos.x + 16, mousePos.y + 16, itemRec.width, itemRec.height}, {0, 0}, 0.0f, WHITE);*/
				/*}*/
				/*else {*/
				/*	DrawTexturePro(atlas.texture, itemTexture, itemRec, {0, 0}, 0.0f, WHITE);*/
				/*	DrawText(std::to_string(playerInventory.stacks[i + startIndexSlot]).c_str(), itemRec.x + 5, itemRec.y + itemRec.height - 15, 20, RAYWHITE);*/
				/*}*/
				DrawTexturePro(atlas.texture, itemTexture, itemRec, {0, 0}, 0.0f, WHITE);
				DrawText(std::to_string(playerInventory.stacks[i + startIndexSlot]).c_str(), itemRec.x + 5, itemRec.y + itemRec.height - 15, 20, RAYWHITE);
			}
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
