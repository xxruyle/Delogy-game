#pragma once
#include "components.hpp"
#include "input_system.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include <entt/entity/registry.hpp>

class InputSystem;

// includes collision detection
class MovementSystem {
public:
  Color collisionColor = WHITE;
  void updatePhysics(PhysicsC &physics, PositionC &position);
  void updatePosition(PhysicsC &physics, CollisionC &collision,
                      PositionC &position, TileManager &tileManager);
  float moveX(int amount, PhysicsC &physics, CollisionC &collision,
              PositionC &position, TileManager &tileManager);

  float moveY(int amount, PhysicsC &physics, CollisionC &collision,
              PositionC &position, TileManager &tileManager);

  bool isCollided(PhysicsC &physics, CollisionC &collision,
                  Vector2 futurePosition, TileManager &tileManager);
  void update(entt::entity player, entt::basic_registry<> &sceneRegistry,
              TileManager &tileManager);
};
