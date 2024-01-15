#include "collision_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "input_system.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include <vector>
#include <iostream>

void CollisionSystem::handleCollision(CollisionC &collision, PhysicsC &physics) {}

bool CollisionSystem::checkCollision(TileManager &tileManager, Vector2 playerPos, Rectangle playerCollisionRec)
{
    Vector2 playerGridPos = getGridPosition(playerPos);
    std::vector<Vector2> gridPositions = tileManager.getNeighbors(playerGridPos.x, playerGridPos.y, 4);

    for (Vector2 position : gridPositions) {
        Vector2 indexPair = tileManager.getIndexPair(position.x * 16, position.y * 16);
        int id = tileManager.chunks[(int)indexPair.x].tileID[(int)indexPair.y];
        /* std::cout << position.x << " " << position.y << std::endl; */

        if (id == 0) {
            Rectangle tilePosition = {position.x * 16, position.y * 16, 16, 16};
            if (CheckCollisionRecs(tilePosition, playerCollisionRec)) {
                /* std::cout << id << ", " << position.x << " " << position.y << ": " */
                /*           << "yes" << std::endl; */
                return true;
            }
        }
    }

    return false;
}

void CollisionSystem::update(Scene &scene, TileManager &tileManager)
{
    if (InputSystem::getUserKeypress() == DEBUG_WIREFRAME) {
        showWireFrame ? showWireFrame = false : showWireFrame = true;
    }

    auto view = scene.EntityRegistry.view<PositionC, PhysicsC, CollisionC>();

    for (auto entity : view) {
        auto &position = view.get<PositionC>(entity);
        auto &physics = view.get<PhysicsC>(entity);
        auto &collision = view.get<CollisionC>(entity);

        Rectangle collisionRec = Rectangle{position.pos.x + collision.aabb.x, position.pos.y + collision.aabb.y,
                                           collision.aabb.width, collision.aabb.height};

        Color collisionColor = WHITE;
        if (checkCollision(tileManager, scene.playerPosition, collisionRec)) {
            collisionColor = GREEN;
        }
        else {
            collisionColor = WHITE;
        }

        if (showWireFrame) {
            DrawRectangleLinesEx(collisionRec, 0.2f, collisionColor);
        }
    }
}
