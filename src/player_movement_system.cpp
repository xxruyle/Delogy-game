#include "player_movement_system.hpp"
#include "input_system.hpp"

void PlayerMovementSystem::updatePhysics(PhysicsC &physics, PositionC &position)
{

    Vector2 movementDirection = InputSystem::getDirectionVector();
    physics.velocity.x = movementDirection.x * physics.speed;
    physics.velocity.y = movementDirection.y * physics.speed;

    if (physics.velocity.x == 0 && physics.velocity.y == 0) {
        physics.moving = false;
    }
    else {
        physics.moving = true;
    }
}

void PlayerMovementSystem::updatePosition(PhysicsC &physics, PositionC &position)
{
    position.pos.x += physics.velocity.x * GetFrameTime();
    position.pos.y += physics.velocity.y * GetFrameTime();
}

void PlayerMovementSystem::update(entt::entity player, entt::basic_registry<> &sceneRegistry)
{
    // getting player physics and position components
    PhysicsC &physics = sceneRegistry.get<PhysicsC>(player);
    PositionC &position = sceneRegistry.get<PositionC>(player);

    updatePhysics(physics, position);
    updatePosition(physics, position);
}
