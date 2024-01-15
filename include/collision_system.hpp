#pragma once
#include "entt/entity/registry.hpp"
#include "scene.hpp"
#include "tile_manager.hpp"

class CollisionSystem {
  public:
    bool showWireFrame = true;
    void handleCollision(CollisionC &collision, PhysicsC &physics);
    bool checkCollision(TileManager &tileManager, Vector2 playerPos, Rectangle playerCollisionRec);
    void update(Scene &scene, TileManager &tileManager);
};
