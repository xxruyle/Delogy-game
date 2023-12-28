#pragma once
#include "raylib.h"

#define GRID_HIGHLIGHT true // will gihlight grid recs on mouse over if true

int UIRowGridRec(Rectangle cellSrc, float thickness, float spacing, int numCells, Color color,
                 Color backgroundColor); // draws one row of grid and returns a index of
                                         // current grid that the mouse is hovering over
