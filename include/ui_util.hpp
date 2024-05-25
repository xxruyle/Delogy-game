#pragma once
#include "atlas.hpp"
#include "components.hpp"
#include "raylib.h"

#define GRID_HIGHLIGHT true // will gihlight grid recs on mouse over if true

// TODO Put this in UI instead of here (note: this means that ui row grid rec
// should be in ui.cpp as well)
namespace UIEvent {
struct UIHoldIcon {       // an icon that is being held
  int slotIndex;          // which slot in the inventory
  Rectangle iconAtlasSrc; // where the icon is located in the atlas
  Vector2 screenPos;      // where the icon is in screen space
  bool active;            // whether or not this event is active
};

struct UIDropIcon {
  int holdedSlotIndex; // want a copy instead of a ptr
  int slotIndex;
  bool active;
};

extern UIHoldIcon holded;
extern UIDropIcon dropped;
}; // namespace UIEvent

int UIRowGridRec(
    Rectangle cellSrc, float thickness, float spacing, int numCells,
    int cellSelected, Color color,
    Color backgroundColor); // draws one row of grid and returns a index of

void UIRowGridIcon(Atlas &atlas, InventoryC &playerInventory, Rectangle cellSrc,
                   float thickness, float spacing, int startIndexSlot,
                   int numCells);
void UIRowGridNumbers(Rectangle cellSrc, float spacing, int numCells);
void UIWindowOutline(Rectangle cellSrc);
