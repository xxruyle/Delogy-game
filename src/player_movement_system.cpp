#include "player_movement_system.hpp"
#include "components.hpp"
#include "input_system.hpp"
#include "macros_util.hpp"

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

void PlayerMovementSystem::updateDirection(DirectionStateC &direction, PhysicsC &physics)
{
    if (physics.velocity.x > 0) {
        direction.curDirection = (int)directionState::EAST;
    }
    else if (physics.velocity.x < 0) {
        direction.curDirection = (int)directionState::WEST;
    }

    if (physics.velocity.y > 0) {
        direction.curDirection = (int)directionState::SOUTH;
    }
    else if (physics.velocity.y < 0) {
        direction.curDirection = (int)directionState::NORTH;
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
    DirectionStateC &direction = sceneRegistry.get<DirectionStateC>(player);
    CollisionC &collision = sceneRegistry.get<CollisionC>(player);

    if (!physics.isCollided) {
        updatePhysics(physics, position);
    }

    updateDirection(direction, physics);
    updatePosition(physics, position);
}
