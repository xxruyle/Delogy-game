#pragma once
#include "atlas.hpp"
#include "components.hpp"
#include "raylib.h"

#define GRID_HIGHLIGHT true // will gihlight grid recs on mouse over if true

int UIRowGridRec(Rectangle cellSrc, float thickness, float spacing, int numCells, int cellSelected, Color color,
                 Color backgroundColor); // draws one row of grid and returns a index of

void UIRowGridIcon(Atlas &atlas, InventoryC &playerInventory, Rectangle cellSrc, float thickness, float spacing,
                   int numCells);

void UIRowGridNumbers(Rectangle cellSrc, float spacing, int numCells);
