#pragma once
#include "entt/entity/registry.hpp"
#include "tile_manager.hpp"

class CollisionSystem {
  public:
    bool showWireFrame = true;
    void checkCollision(TileManager &tileManager);
    void update(entt::basic_registry<> &registry);
};
