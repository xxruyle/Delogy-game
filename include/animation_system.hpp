#pragma once
#include "components.hpp"
#include "input_system.hpp"
#include <entt/entity/registry.hpp>

enum class animationDirection { RIGHT = 0, LEFT, UP, DOWN, RIGHT_IDLE, LEFT_IDLE, UP_IDLE, DOWN_IDLE };
class AnimationSystem {
  public:
    void updatePlayerAnimation(InputSystem input, AnimationC &animation, bool movementState);
    void updateNPCAnimations(AnimationC &animation, PhysicsC &physics);
    void incrementAnimation(AnimationC &animation);
    void updateSprites(entt::basic_registry<> &registry);
    void update(InputSystem input, entt::basic_registry<> &registry, entt::entity player);
};
