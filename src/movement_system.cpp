#include "movement_system.hpp"
#include "dev_util.hpp"
#include "components.hpp"
#include "input_system.hpp"
#include "macros_util.hpp"
#include "lua/lualoader.hpp"
#include "raylib.h"
#include <cmath>
#include <iostream>

void MovementSystem::updatePhysics(PhysicsC& physics, PositionC& position)
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

void MovementSystem::updateNPCPhysics(PhysicsC& physics, PositionC& position)
{ // update npc velocity
}

bool MovementSystem::isCollided(PhysicsC& physics, CollisionC& collision, Vector2 futurePosition, TileManager& tileManager)
{
    Vector2 playerGridPos = getGridPosition(futurePosition);
    std::vector<Vector2> gridPositions = tileManager.getNeighbors(playerGridPos.x, playerGridPos.y, 4);

    Rectangle playerCollisionRec = {futurePosition.x + collision.aabb.x, futurePosition.y + collision.aabb.y, collision.aabb.width, collision.aabb.height};

    for (Vector2 position : gridPositions) {
        IndexPair indexPair = tileManager.getIndexPair(position.x * 16, position.y * 16);
        int id = tileManager.chunks[indexPair.chunk].tileZ[indexPair.tile];

        if (id == 1) {
            Rectangle tileRec = {(position.x * 16), (position.y * 16), 16, 16};
            if (CheckCollisionRecs(tileRec, playerCollisionRec)) {
                return true;
            }
        }
    }

    return false;
}

float MovementSystem::moveX(int amount, PhysicsC& physics, CollisionC& collision, PositionC& position, TileManager& tileManager)
{
    float remainderX = 0.0f;
    float step = 3.0f;

    int sign = 1;
    if (amount < 0) {
        sign = -1;
    }

    if (amount != 0) {
        while (abs(remainderX * GetFrameTime()) <= abs(amount * GetFrameTime())) {
            if (!isCollided(physics, collision, Vector2{position.pos.x + ((((step + 1.0f) * sign) + remainderX) * GetFrameTime()), position.pos.y}, tileManager) || !physics.is_solid || !collisionEnabled) {
                collisionColor = WHITE;
                remainderX += step * sign;
            }
            else {
                collisionColor = RED;
                break;
            }
        }
    }

    return remainderX;
}

float MovementSystem::moveY(int amount, PhysicsC& physics, CollisionC& collision, PositionC& position, TileManager& tileManager)
{
    float remainderY = 0.0f;
    float step = 3.0f;

    int sign = 1;
    if (amount < 0) {
        sign = -1;
    }

    if (amount != 0) {
        while (abs(remainderY * GetFrameTime()) <= abs(amount * GetFrameTime())) {
            if (!isCollided(physics, collision, Vector2{position.pos.x, (position.pos.y + ((((step + 1.0f) * sign) + remainderY) * GetFrameTime()))}, tileManager) || !physics.is_solid || !collisionEnabled) {
                collisionColor = WHITE;
                remainderY += step * sign;
            }
            else {
                collisionColor = RED;
                break;
            }
        }
    }

    return remainderY;
}

void MovementSystem::updatePosition(PhysicsC& physics, CollisionC& collision, PositionC& position, TileManager& tileManager)
{
    position.pos.x += moveX(physics.velocity.x, physics, collision, position, tileManager) * GetFrameTime();
    position.pos.y += moveY(physics.velocity.y, physics, collision, position, tileManager) * GetFrameTime();
}

void MovementSystem::updateNPCPositions(PhysicsC& physics, CollisionC& collision, PositionC& position, TileManager& tileManager)
{
    position.pos.x += physics.velocity.x * GetFrameTime();
    position.pos.y += physics.velocity.y * GetFrameTime();

    if (physics.velocity.x == 0 && physics.velocity.y == 0) {
        physics.moving = false;
    }
    else {
        physics.moving = true;
    }
}

void MovementSystem::update(entt::entity player, entt::basic_registry<>& sceneRegistry, TileManager& tileManager)
{

    auto view = sceneRegistry.view<PositionC, PhysicsC, CollisionC>();

    for (auto entity : view) {

        PhysicsC& physics = view.get<PhysicsC>(entity);
        PositionC& position = view.get<PositionC>(entity);
        CollisionC& collision = view.get<CollisionC>(entity);

        if (entity == player) {
            updatePhysics(physics, position);
            updatePosition(physics, collision, position, tileManager);
        }
        else {
            /* updateNPCPhysics(physics, position); */
            updateNPCPositions(physics, collision, position, tileManager);
        }
    }
}
