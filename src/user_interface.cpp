#include "user_interface.h"
#include "raylib.h"

int UIRowGridRec(Rectangle cellSrc, float thickness, float spacing, int numCells, Color color, Color backgroundColor)
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
                tempBackground.a = 200;
                DrawRectangleRec(cellSrc, tempBackground); // Drawing the background
            }
            else {
                DrawRectangleRec(cellSrc, backgroundColor); // Drawing the background
            }
        }
        else {
            DrawRectangleRec(cellSrc, backgroundColor); // Drawing the background
        }
    }
    return result; // returning the cell a mouse position was in
}
