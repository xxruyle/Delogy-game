#include "player_movement_system.hpp"
#include "dev_util.hpp"
#include "components.hpp"
#include "input_system.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include <cmath>
#include <iostream>

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

bool PlayerMovementSystem::isCollided(PhysicsC &physics, CollisionC &collision, Vector2 futurePosition,
                                      TileManager &tileManager)
{
    Vector2 playerGridPos = getGridPosition(futurePosition);
    std::vector<Vector2> gridPositions = tileManager.getNeighbors(playerGridPos.x, playerGridPos.y, 4);

    Rectangle playerCollisionRec = {futurePosition.x + collision.aabb.x, futurePosition.y + collision.aabb.y,
                                    collision.aabb.width, collision.aabb.height};

    for (Vector2 position : gridPositions) {
        IndexPair indexPair = tileManager.getIndexPair(position.x * 16, position.y * 16);
        int id = tileManager.chunks[(int)indexPair.x].tileID[(int)indexPair.y];

        if (id == 0) {
            Rectangle tileRec = {(position.x * 16), (position.y * 16), 16, 16};
            if (CheckCollisionRecs(tileRec, playerCollisionRec)) {
                return true;
            }
        }
    }

    return false;
}

float PlayerMovementSystem::moveX(int amount, PhysicsC &physics, CollisionC &collision, PositionC &position,
                                  TileManager &tileManager)
{
    float remainderX = 0.0f;

    int sign = 1;
    if (amount < 0) {
        sign = -1;
    }

    if (amount != 0) {
        while (abs(remainderX) <= abs(amount)) {
            if (!isCollided(physics, collision, Vector2{position.pos.x + sign, position.pos.y}, tileManager) ||
                !COLLISION_ENABLED) {
                collisionColor = WHITE;
                remainderX += 0.1f * sign;
            }
            else {
                collisionColor = RED;
                break;
            }
        }
    }

    return remainderX;
}

float PlayerMovementSystem::moveY(int amount, PhysicsC &physics, CollisionC &collision, PositionC &position,
                                  TileManager &tileManager)
{
    float remainderY = 0.0f;

    int sign = 1;
    if (amount < 0) {
        sign = -1;
    }

    if (amount != 0) {
        while (abs(remainderY) <= abs(amount)) {
            if (!isCollided(physics, collision, Vector2{position.pos.x, position.pos.y + sign}, tileManager) ||
                !COLLISION_ENABLED) {
                collisionColor = WHITE;
                remainderY += 0.1f * sign;
            }
            else {
                collisionColor = RED;
                break;
            }
        }
    }

    return remainderY;
}

void PlayerMovementSystem::updatePosition(PhysicsC &physics, CollisionC &collision, PositionC &position,
                                          TileManager &tileManager)
{

    position.pos.x += moveX(physics.velocity.x, physics, collision, position, tileManager) * GetFrameTime();
    position.pos.y += moveY(physics.velocity.y, physics, collision, position, tileManager) * GetFrameTime();

    DrawRectangleLinesEx(Rectangle{position.pos.x + collision.aabb.x, position.pos.y + collision.aabb.y,
                                   collision.aabb.width, collision.aabb.height},
                         0.2f, collisionColor);
}

void PlayerMovementSystem::update(entt::entity player, entt::basic_registry<> &sceneRegistry, TileManager &tileManager)
{
    // getting player physics and position components
    PhysicsC &physics = sceneRegistry.get<PhysicsC>(player);
    PositionC &position = sceneRegistry.get<PositionC>(player);
    DirectionStateC &direction = sceneRegistry.get<DirectionStateC>(player);
    CollisionC &collision = sceneRegistry.get<CollisionC>(player);

    updatePhysics(physics, position);
    updateDirection(direction, physics);
    updatePosition(physics, collision, position, tileManager);
}
