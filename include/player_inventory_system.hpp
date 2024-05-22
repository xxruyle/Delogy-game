#pragma once
#include "atlas.hpp"
#include "components.hpp"
#include "scene.hpp"

class PlayerInventorySystem {
  public:
    void updateInventorySelection(InventoryC& inventory); // switch inventory selection
    void updateItemRotation(InventoryC& inventory);
    void drawCurItem(Atlas& atlas, Camera2D& camera, InventoryC& inventory);
    void update(Scene& scene);
};
