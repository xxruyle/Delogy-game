#pragma once
#include "components.hpp"
#include "input_system.hpp"
#include <entt/entity/registry.hpp>

class InputSystem;
class PlayerMovementSystem {
  public:
    void updatePhysics(InputSystem input, PhysicsC &physics, PositionC &position);
    void updatePosition(InputSystem input, PhysicsC &physics, PositionC &position);
    void update(InputSystem input, entt::entity player, entt::basic_registry<> &sceneRegistry);
};
