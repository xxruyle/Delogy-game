#pragma once
#include "atlas.hpp"
#include "components.hpp"
#include "input_system.hpp"
#include "scene.hpp"

class PlayerInventorySystem {
  public:
    void updateInventorySelection(InventoryC &inventory, InputSystem &input); // switch inventory selection
    void updateItemRotation(InventoryC &inventory, InputSystem &input);
    void drawCurItem(Atlas &atlas, Camera2D &camera, InventoryC &inventory);
    void update(Scene &scene, InputSystem &input);
};
