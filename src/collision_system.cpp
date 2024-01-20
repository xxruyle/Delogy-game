#include "collision_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "input_system.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include <algorithm>
#include <limits>
#include <vector>
#include <iostream>

float CollisionSystem::sweptAABB(CollisionC &collision, PhysicsC &physics, Rectangle playerCollisionRec,
                                 Rectangle targetCollision)
{
    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;

    if (physics.velocity.x > 0.0f) {
        xInvEntry = targetCollision.x - (playerCollisionRec.x + playerCollisionRec.width);
        xInvExit = (targetCollision.x + targetCollision.width) - playerCollisionRec.x;
    }
    else {
        xInvEntry = (targetCollision.x + targetCollision.width) - playerCollisionRec.x;
        xInvExit = targetCollision.x - (playerCollisionRec.x + playerCollisionRec.width);
    }

    if (physics.velocity.y > 0.0f) {
        yInvEntry = targetCollision.y - (playerCollisionRec.y + playerCollisionRec.height);
        yInvExit = (targetCollision.y + targetCollision.height) - playerCollisionRec.y;
    }
    else {
        yInvEntry = (targetCollision.y + targetCollision.height) - playerCollisionRec.y;
        yInvExit = targetCollision.y - (playerCollisionRec.y + playerCollisionRec.height);
    }

    float xEntry, yEntry;
    float xExit, yExit;

    if (physics.velocity.x == 0.0f) {
        xEntry = -std::numeric_limits<float>::max();
        xExit = std::numeric_limits<float>::max();
    }
    else {
        xEntry = xInvEntry / physics.velocity.x;
        xExit = xInvExit / physics.velocity.x;
    }

    if (physics.velocity.y == 0.0f) {
        yEntry = -std::numeric_limits<float>::max();
        yExit = std::numeric_limits<float>::max();
    }
    else {
        yEntry = yInvEntry / physics.velocity.y;
        yExit = yInvExit / physics.velocity.y;
    }

    float entryTime = std::max(xEntry, yEntry);
    float exitTime = std::min(xExit, yExit);

    float normalX;
    float normalY;

    if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f) {
        collision.normal = Vector2{0.0f, 0.0f};
        return 1.0f;
    }
    else {
        if (xEntry > yEntry) {
            if (xInvEntry < 0.0f) {
                collision.normal = Vector2{1.0f, 0.0f};
            }
            else {
                collision.normal = Vector2{-1.0f, 0.0f};
            }
        }
        else {
            if (yInvEntry < 0.0f) {
                collision.normal = Vector2{0.0f, 1.0f};
            }
            else {
                collision.normal = Vector2{0.0f, -1.0f};
            }
        }
    }

    return entryTime;
}

void CollisionSystem::collisionResponse(DirectionStateC &direction, PhysicsC &physics, PositionC &position,
                                        Rectangle playerCollisionRec, Rectangle targetCollision)
{
    playerCollisionRec =
        Rectangle{playerCollisionRec.x, playerCollisionRec.y, playerCollisionRec.x + playerCollisionRec.width,
                  playerCollisionRec.y + playerCollisionRec.height};

    targetCollision = Rectangle{targetCollision.x, targetCollision.y, targetCollision.x + targetCollision.width,
                                targetCollision.y + targetCollision.height};

    float overlapX =
        std::min(abs(playerCollisionRec.width - targetCollision.x), abs(targetCollision.width - playerCollisionRec.x));

    float overlapY = std::min(abs(playerCollisionRec.height - targetCollision.y),
                              abs(targetCollision.height - playerCollisionRec.y));

    Vector2 collisionNormal;
    if (overlapX < overlapY) {
        if ((playerCollisionRec.width - targetCollision.x) < (targetCollision.width - playerCollisionRec.x)) {
            collisionNormal = Vector2{-1.0f, 0.0f};
        }
        else {
            collisionNormal = Vector2{1.0f, 0.0f};
        }
    }
    else {
        if ((playerCollisionRec.height - targetCollision.y) < (targetCollision.height - playerCollisionRec.y)) {
            collisionNormal = Vector2{0.0f, -1.0f};
        }
        else {
            collisionNormal = Vector2{0.0f, 1.0f};
        }
    }

    float collisionOffset = 0.35f * GetFrameTime();
    float potentialOffset;
    float offsetSpeed = -0.2;
    /* std::cout << collisionNormal.x << " " << collisionNormal.y << std::endl; */

    if (collisionNormal.x == -1.0f && collisionNormal.y == 0.0f && physics.velocity.x >= 0.0f) {
        potentialOffset = playerCollisionRec.width - targetCollision.x;
        physics.velocity.x *= offsetSpeed;

        /* position.pos = Vector2{position.pos.x - collisionOffset, position.pos.y}; */
    }
    else if (collisionNormal.x == 1.0f && collisionNormal.y == 0.0f && physics.velocity.x <= 0.0f) {
        potentialOffset = targetCollision.width - playerCollisionRec.x;
        physics.velocity.x *= offsetSpeed;
        /* position.pos = Vector2{position.pos.x + collisionOffset, position.pos.y}; */
    }

    if (collisionNormal.x == 0.0f && collisionNormal.y == -1.0f && physics.velocity.y >= 0.0f) {
        potentialOffset = playerCollisionRec.height - targetCollision.y;
        physics.velocity.y *= offsetSpeed;
        /* position.pos = Vector2{position.pos.x, position.pos.y - collisionOffset}; */
    }
    else if (collisionNormal.x == 0.0f && collisionNormal.y == 1.0f && physics.velocity.y <= 0.0f) {
        potentialOffset = targetCollision.height - playerCollisionRec.y;
        physics.velocity.y *= offsetSpeed;
        /* position.pos = Vector2{position.pos.x, position.pos.y + collisionOffset}; */
    }

    /* std::cout << physics.velocity.x << " " << physics.velocity.y << std::endl; */
}

bool CollisionSystem::checkCollision(TileManager &tileManager, Vector2 playerPos, Rectangle playerCollisionRec,
                                     CollisionC &playerCollision, PhysicsC &playerPhysics, PositionC &playerPosition,
                                     DirectionStateC &direction)
{
    Vector2 playerGridPos = getGridPosition(playerPos);
    std::vector<Vector2> gridPositions = tileManager.getNeighbors(playerGridPos.x, playerGridPos.y, 4);

    for (Vector2 position : gridPositions) {
        IndexPair indexPair = tileManager.getIndexPair(position.x * 16, position.y * 16);
        int id = tileManager.chunks[(int)indexPair.x].tileID[(int)indexPair.y];
        /* std::cout << position.x << " " << position.y << std::endl; */

        float collisionTime;
        if (id == 0) {
            Rectangle tileRec = {position.x * 16, position.y * 16, 16, 16};
            if (CheckCollisionRecs(tileRec, playerCollisionRec)) {
                /* std::cout << tileRec.x / 16 << " " << tileRec.y / 16 << std::endl; */
                collisionResponse(direction, playerPhysics, playerPosition, playerCollisionRec, tileRec);
                playerPhysics.isCollided = true;
                return true;
            }
        }
    }
    playerPhysics.isCollided = false;
    return false;
}

void CollisionSystem::update(Scene &scene, TileManager &tileManager)
{
    if (InputSystem::getUserKeypress() == DEBUG_WIREFRAME) {
        showWireFrame ? showWireFrame = false : showWireFrame = true;
    }

    auto view = scene.EntityRegistry.view<PositionC, PhysicsC, CollisionC, DirectionStateC>();

    for (auto entity : view) {
        auto &position = view.get<PositionC>(entity);
        auto &physics = view.get<PhysicsC>(entity);
        auto &collision = view.get<CollisionC>(entity);
        auto &direction = view.get<DirectionStateC>(entity);

        Rectangle collisionRec = Rectangle{position.pos.x + collision.aabb.x, position.pos.y + collision.aabb.y,
                                           collision.aabb.width, collision.aabb.height};

        if (showWireFrame) {
            DrawRectangleLinesEx(collisionRec, 0.2f, collisionColor);
        }
        bool result =
            checkCollision(tileManager, scene.playerPosition, collisionRec, collision, physics, position, direction);
        result ? collisionColor = RED : collisionColor = WHITE;
    }
}
