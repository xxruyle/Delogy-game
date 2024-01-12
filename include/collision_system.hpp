#pragma once
#include "entt/entity/registry.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"

class CollisionSystem {
  public:
    bool showWireFrame = true;
    void checkCollision(TileManager &tileManager);
    void update(entt::basic_registry<> &registry);
};
