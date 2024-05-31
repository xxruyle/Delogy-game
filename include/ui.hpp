#include "atlas.hpp"
#include "components.hpp"
#include "draw_system.hpp"
#include "ui_util.hpp"

enum BoundType { HOTBAR, MINIMAP };

struct DragAndDropper {
  InventoryC *inventoryHeld;
  InventoryC *inventoryDropped;
  int holdedSlotIndex;    // which inventory index slot is held
  int droppedSlotIndex;   // which inventory index slot the icon is dropped on
  Rectangle iconAtlasSrc; // where the icon is in the atlas
  bool holdActive;        // something is held
  bool dropActive;        // something is being dropped
};

class UI {
public:
  SpriteDrawSystem *drawSystem;
  UI(SpriteDrawSystem *spriteDrawSystem);
  Rectangle bounds[2];

  // drag and drop behavior tracker
  DragAndDropper dragAndDrop = {nullptr, nullptr, 0, 0, Rectangle{0, 0, 0, 0},
                                false,   false};

  bool mouseOutOfBounds(); // check if mouse is not in the ui bound space
  const float gridSpacing = 2.0f;
  void hotBar(InventoryC &inventory, HotBarC &hotBar, Vector2 src, int width,
              int height);
  void inventory(InventoryC &inventory, Vector2 src, int width, int height,
                 int cellsPerRow);
  void miniMap(Rectangle mapSrc);

private:
  void inventoryRowSlots(InventoryC &inventory, int curRow, int iconSize,
                         int numCells, Rectangle rowCellSrc);
  void handleDropEvent(InventoryC &inventory);
  void checkDragAndDrop(InventoryC &inventory, int inventoryIndex,
                        int gridValidity);
  void checkResetDrop(
      bool cellFound); // player dropped and item on an invalid location
};
