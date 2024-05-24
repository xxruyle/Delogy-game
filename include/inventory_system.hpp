#pragma once
#include "atlas.hpp"
#include "components.hpp"
#include "scene.hpp"

class InventorySystem {
public:
  void swapItem(InventoryC &inventory, int index1, int index2);

  void updateInventorySelection(InventoryC &inventory,
                                HotBarC &hotBar); // switch inventory selection

  void updateItemRotation(InventoryC &inventory, HotBarC &hotBar);
  void drawCurItem(Atlas &atlas, Camera2D &camera, InventoryC &inventory,
                   HotBarC &hotBar);
  void update(Scene &scene);
};
