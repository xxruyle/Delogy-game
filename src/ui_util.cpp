#include "ui_util.hpp"
#include "input_system.hpp"
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

void UIIcon(Atlas& atlas, Rectangle atlasSrc, Rectangle iconSrc)
{
	// awdawd
	DrawTexturePro(atlas.texture, atlasSrc, iconSrc, {0.0f, 0.0f}, 0.0f, WHITE);
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
