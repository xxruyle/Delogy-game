#pragma once
#include "components.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "tile_manager.hpp"

class CollisionSystem {
  public:
    bool showWireFrame = true;
    void collisionResponse(PhysicsC &physics, PositionC &position, Rectangle playerCollisionRec,
                           Rectangle targetCollision);
    float sweptAABB(CollisionC &collision, PhysicsC &physics, Rectangle playerCollisionRec, Rectangle targetCollision);
    bool checkCollision(TileManager &tileManager, Vector2 playerPos, Rectangle playerCollisionRec,
                        CollisionC &playerCollision, PhysicsC &playerPhysics, PositionC &playerPosition);
    void update(Scene &scene, TileManager &tileManager);
};
