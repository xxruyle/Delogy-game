#pragma once
#include "components.hpp"
#include "input_system.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include <entt/entity/registry.hpp>

class InputSystem;
class PlayerMovementSystem {
  public:
    void updatePhysics(PhysicsC &physics, PositionC &position);
    void updateDirection(DirectionStateC &direction, PhysicsC &physics);
    void updatePosition(PhysicsC &physics, PositionC &position);
    void update(entt::entity player, entt::basic_registry<> &sceneRegistry);
};
