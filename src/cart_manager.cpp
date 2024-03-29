#include "cart_manager.hpp"
#include "game_time.hpp"
#include "input_system.hpp"
#include "item_data.hpp"
#include "raylib.h"
#include "macros_util.hpp"
#include "entity_data.hpp"
#include "dev_util.hpp"
#include <iostream>

std::unordered_set<int> CartManager::getValidRails(int rail, int direction)
{
    std::unordered_set<int> validRails = {};

    switch (rail) {
    case RAIL_NW:
        if (direction == EAST) {
            validRails = {RAIL_H, RAIL_NE, RAIL_SE};
        }
        else if (direction == SOUTH) {
            validRails = {RAIL_V, RAIL_SE, RAIL_SW};
        }
        break;
    case RAIL_NE:
        if (direction == WEST) {
            validRails = {RAIL_H, RAIL_NW, RAIL_SW};
        }
        else if (direction == SOUTH) {
            validRails = {RAIL_V, RAIL_SE, RAIL_SW};
        }
        break;
    case RAIL_SE:
        if (direction == NORTH) {
            validRails = {RAIL_V, RAIL_NW, RAIL_NE};
        }
        else if (direction == WEST) {
            validRails = {RAIL_H, RAIL_NW, RAIL_SW};
        }
        break;
    case RAIL_SW:
        if (direction == NORTH) {
            validRails = {RAIL_V, RAIL_NW, RAIL_NE};
        }
        else if (direction == EAST) {
            validRails = {RAIL_H, RAIL_NE, RAIL_SE};
        }
        break;
    case RAIL_H:
        if (direction == WEST) {
            validRails = {RAIL_H, RAIL_NW, RAIL_SW};
        }
        else if (direction == EAST) {
            validRails = {RAIL_H, RAIL_NE, RAIL_SE};
        }
        break;
    case RAIL_V:
        if (direction == NORTH) {
            validRails = {RAIL_V, RAIL_NW, RAIL_NE};
        }
        else if (direction == SOUTH) {
            validRails = {RAIL_V, RAIL_SE, RAIL_SW};
        }
        break;
    default:
        validRails = {};
        break;
    }

    return validRails;
}

int CartManager::getDirectionMultiplier(int direction)
{
    switch (direction) {
    case WEST:
        return -1;
    case EAST:
        return 1;
    case NORTH:
        return -1;
    case SOUTH:
        return 1;
    default:
        return 1;
    }
}

Vector2 CartManager::getFarSideCartBorder(PositionC &position, int direction)
{
    Vector2 cartPos;
    switch (direction) {
    case WEST:
        cartPos = {position.pos.x + 15, position.pos.y + 8}; // put position on the right side
        break;
    case EAST:
        cartPos = {position.pos.x, position.pos.y + 8};
        break;
    case NORTH:
        cartPos = {position.pos.x + 8, position.pos.y + 15}; // put position on the right side
        break;
    case SOUTH:
        cartPos = {position.pos.x, position.pos.y}; // put position on the right side
        break;
    default:
        cartPos = position.pos;
        break;
    }
    return cartPos;
}

Vector2 CartManager::getNearSideCartBorder(PositionC &position, int direction)
{
    Vector2 cartPos;
    switch (direction) {
    case WEST:
        cartPos = {position.pos.x + 1.0f, position.pos.y + 8.0f};
        break;
    case EAST:
        cartPos = {position.pos.x + 15.0f, position.pos.y + 8.0f};
        break;
    case NORTH:
        cartPos = {position.pos.x + 8.0f, position.pos.y};
        break;
    case SOUTH:
        cartPos = {position.pos.x, position.pos.y + 15.0f};
        break;
    default:
        cartPos = position.pos;
        break;
    }
    return cartPos;
}

void CartManager::getPlayerInteraction(InventoryC &inventory, Camera2D &camera, entt::basic_registry<> &registry)
{
    if (InputSystem::getUserMouseInteraction() == PLAYER_CREATE) {
        int inventoryItem = inventory.hotbar[inventory.curItem];
        if (inventoryItem == CART) {
            Vector2 mouseGridPos = getMouseGridPosition(camera);
            createCart(mouseGridPos, registry);
        };
    }
}

void CartManager::changeCartDirection(PositionC &position, OrecartC &orecart, PhysicsC &physics, int itemUnder,
                                      int prevItemUnder, int futureItemUnder)
{
    Vector2 curGridPos = getGridPosition({position.pos.x + 8.0f, position.pos.y + 8.0f});

    if (prevItemUnder != NULL_ITEM && futureItemUnder != NULL_ITEM) {
        if (orecart.previousGridPos.x != curGridPos.x || orecart.previousGridPos.y != curGridPos.y ||
            !physics.moving) { // recheck if the cart is stopped

            std::unordered_set<int> connectableRails = getValidRails(
                prevItemUnder,
                orecart.movementDirection); // get all the connectable rails for the previous rail the cart was under

            if (!connectableRails.count(
                    futureItemUnder)) { // if the future rail that the cart is going on is connectable
                physics.moving = false;
                physics.velocity.x = 0;
                physics.velocity.y = 0;
            }
            else {
                physics.moving = true;
            }
        }
    }

    if (physics.moving) {
        orecart.previousGridPos = curGridPos;
    }

    /* std::cout << prevItemUnder << " " << itemUnder << " " << futureItemUnder << " " << orecart.movementDirection */
    /*           << std::endl; */

    switch (itemUnder) {
    case RAIL_H:
        if (orecart.movementDirection == SOUTH) {
            orecart.movementDirection = EAST;
        }
        else if (orecart.movementDirection == NORTH) {
            orecart.orientation = WEST;
        }
        orecart.previousRail = RAIL_H;
        break;
    case RAIL_NE:
        if (orecart.movementDirection == EAST) {
            physics.velocity.x = 0;
            orecart.movementDirection = SOUTH;
        }
        else if (orecart.movementDirection == NORTH) {
            physics.velocity.y = 0;
            orecart.movementDirection = WEST;
        }
        orecart.previousRail = RAIL_NE;
        break;
    case RAIL_V:
        if (orecart.movementDirection == EAST) {
            orecart.movementDirection = SOUTH;
        }
        else if (orecart.movementDirection == WEST) {
            orecart.movementDirection = NORTH;
        }
        orecart.previousRail = RAIL_V;
        break;
    case RAIL_SE:
        if (orecart.movementDirection == SOUTH) {
            physics.velocity.y = 0;
            orecart.movementDirection = WEST;
        }
        else if (orecart.movementDirection == EAST) {
            physics.velocity.x = 0;
            orecart.movementDirection = NORTH;
        }
        orecart.previousRail = RAIL_SE;
        break;
    case RAIL_SW:
        if (orecart.movementDirection == WEST) {
            physics.velocity.x = 0;
            orecart.movementDirection = NORTH;
        }
        else if (orecart.movementDirection == SOUTH) {
            physics.velocity.y = 0;
            orecart.movementDirection = EAST;
        }
        orecart.previousRail = RAIL_SW;
        break;
    case RAIL_NW:
        if (orecart.movementDirection == NORTH) {
            physics.velocity.y = 0;
            orecart.movementDirection = EAST;
        }
        else if (orecart.movementDirection == WEST) {
            physics.velocity.x = 0;
            orecart.movementDirection = SOUTH;
        }
        orecart.previousRail = RAIL_NW;
        break;
    default: // if the item underneat is the NULL_ITEM
        physics.moving = false;
        physics.velocity.x = 0;
        physics.velocity.y = 0;
        break;
    }
    /* std::cout << itemUnder << std::endl; */
}

void CartManager::changeCartVelocity(PhysicsC &physics, OrecartC &orecart)
{

    int directionMultiplier = getDirectionMultiplier(orecart.movementDirection);

    if (physics.moving) {
        switch (orecart.movementDirection) {
        case WEST:
            orecart.orientation = CART_H;
            physics.velocity.x = directionMultiplier * physics.speed;
            break;
        case EAST:
            orecart.orientation = CART_H;
            physics.velocity.x = directionMultiplier * physics.speed;
            break;
        case NORTH:
            orecart.orientation = CART_V;
            physics.velocity.y = directionMultiplier * physics.speed;
            break;
        case SOUTH:
            orecart.orientation = CART_V;
            physics.velocity.y = directionMultiplier * physics.speed;
            break;
        default:
            physics.velocity.x = 0;
            physics.velocity.y = 0;
            break;
        }
    }
    else {
        // change orientation to derailed sprites
        if (orecart.orientation == CART_H) {
            orecart.orientation = CART_HD;
        }
        else if (orecart.orientation == CART_V) {
            orecart.orientation = CART_VD;
        }
        physics.velocity.x = 0;
        physics.velocity.y = 0;
    }
}

void CartManager::changeCartPosition(PositionC &position, PhysicsC &physics)
{
    /* position.pos.x += physics.velocity.x * GameTime::getDT(); */
    /* position.pos.y += physics.velocity.y * GameTime::getDT(); */
}

void CartManager::updateCarts(entt::basic_registry<> &registry, TileManager &tileManager)
{
    auto view = registry.view<SpriteC, PhysicsC, PositionC, OrecartC, CollisionC>();

    for (auto entity : view) {
        auto &sprite = view.get<SpriteC>(entity);
        auto &physics = view.get<PhysicsC>(entity);
        auto &position = view.get<PositionC>(entity);
        auto &orecart = view.get<OrecartC>(entity);
        auto &collision = view.get<CollisionC>(entity);

        Vector2 farSideAABB = getFarSideCartBorder(position, orecart.movementDirection);
        int itemUnder = tileManager.getItemUnder(farSideAABB);
        int prevItemUnder =
            tileManager.getItemUnder({orecart.previousGridPos.x * 16.0f, orecart.previousGridPos.y * 16.0f});
        int futureItemUnder = tileManager.getItemUnder({position.pos.x + 8.0f, position.pos.y + 8.0f});

        changeCartDirection(position, orecart, physics, itemUnder, prevItemUnder, futureItemUnder);
        changeCartVelocity(physics, orecart);
        changeCartPosition(position, physics);

        // change sprite based on cart orientation
        Entity ent = entityids[orecart.orientation];
        sprite.atlasPos = Rectangle{ent.x, ent.y, 16, 16};
    }
}

void CartManager::createCart(Vector2 position, entt::basic_registry<> &registry)
{
    entt::entity entity = registry.create();
    registry.emplace<SpriteC>(entity, AtlasType::SMALL, Rectangle{67, 88, 16, 16});
    registry.emplace<PositionC>(entity, Vector2{position.x * 16, position.y * 16});
    registry.emplace<OrecartC>(entity, CART_H, EAST, NULL_ITEM, position);
    registry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 30, 60, true, true);
    registry.emplace<CollisionC>(entity, Rectangle{4, 4, 8, 8});

    cartCount++;
}
void CartManager::update(TileManager &tileManager, Scene &scene)

{
    getPlayerInteraction(scene.EntityRegistry.get<InventoryC>(scene.player), scene.camera, scene.EntityRegistry);
    updateCarts(scene.EntityRegistry, tileManager);
}
