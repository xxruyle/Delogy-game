#include "entity_manager.hpp"
#include "dev_util.hpp"
#include "entity_data.hpp"
#include "item_data.hpp"
#include "raylib.h"
#include "player.hpp"

EntityPhysics::EntityPhysics(Vector2 gridPos)
{
    gridPos.x *= 16;
    gridPos.y *= 16;
    pos = gridPos;
}

void EntityPhysics::clampSpeed()
{

    if (velocity.x > maxSpeed) {
        velocity.x = maxSpeed;
    }

    if (velocity.x < -maxSpeed) {
        velocity.x = -maxSpeed;
    }

    if (velocity.y > maxSpeed) {
        velocity.y = maxSpeed;
    }

    if (velocity.y < -maxSpeed) {
        velocity.y = -maxSpeed;
    }
}

void EntityPhysics::update()
{
    clampSpeed();
    pos.x += velocity.x * GetFrameTime();
    pos.y += velocity.y * GetFrameTime();
}

void EntityManager::createCart(Vector2 gridPos, int id)
{
    EntityPhysics tempPhysics(gridPos);
    Cart tempCart = {id, CART_H, tempPhysics};
    carts.push_back(tempCart);
}

void EntityManager::populateCarts()
{
    for (int i = 0; i < 1; i++) {
        Vector2 gridPos = {(float)GetRandomValue(0, 2), 0};
        createCart(gridPos, carts.size());
    }
}

void EntityManager::drawCarts(Atlas &atlas)
{
    for (int i = 0; i < carts.size(); i++) {
        Entity curEntity = entityids[carts[i].orientation];
        Rectangle entityRec = {(float)curEntity.x, (float)curEntity.y, 16, 16};
        DrawTextureRec(atlas.texture, entityRec, carts[i].physics_.pos, WHITE);
    }
}
void EntityManager::getCartPaths(TileManager &tileManager) {}

int getDirectionMultiplier(int direction)
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

Vector2 getFarSideCartAABB(Cart &cart)
{
    Vector2 cartPos;
    switch (cart.curDirection) {
    case WEST:
        cartPos = {cart.physics_.pos.x + 15.5f, cart.physics_.pos.y + 8.0f}; // put position on the right side
        break;
    case EAST:
        cartPos = {cart.physics_.pos.x, cart.physics_.pos.y + 8.0f};
        break;
    case NORTH:
        cartPos = {cart.physics_.pos.x + 8.0f, cart.physics_.pos.y + 15.5f}; // put position on the right side
        break;
    case SOUTH:
        cartPos = {cart.physics_.pos.x, cart.physics_.pos.y}; // put position on the right side
        break;
    default:
        cartPos = cart.physics_.pos;
        break;
    }
    return cartPos;
}

Vector2 getNearSideCartAABB(Cart &cart)
{
    Vector2 cartPos;
    switch (cart.curDirection) {
    case WEST:
        cartPos = {cart.physics_.pos.x, cart.physics_.pos.y + 8.0f};
        break;
    case EAST:
        cartPos = {cart.physics_.pos.x + 15.8f, cart.physics_.pos.y + 8.0f};
        break;
    case NORTH:
        cartPos = {cart.physics_.pos.x + 8.0f, cart.physics_.pos.y};
        break;
    case SOUTH:
        cartPos = {cart.physics_.pos.x, cart.physics_.pos.y + 15.8f};
        break;
    default:
        cartPos = cart.physics_.pos;
        break;
    }
    return cartPos;
}

void Cart::updateDirection(int itemUnder)
{
    switch (itemUnder) {
    case RAIL_H:
        if (curDirection == SOUTH) {
            curDirection = EAST;
        }
        else if (curDirection == NORTH) {
            curDirection = WEST;
        }
        break;
    case RAIL_NE:
        if (curDirection == EAST) {
            physics_.velocity.x = 0;
            curDirection = SOUTH;
        }
        else if (curDirection == NORTH) {
            physics_.velocity.y = 0;
            curDirection = WEST;
        }
        break;
    case RAIL_V:
        if (curDirection == EAST) {
            curDirection = SOUTH;
        }
        else if (curDirection == WEST) {
            curDirection = NORTH;
        }
        break;
    case RAIL_SE:
        if (curDirection == SOUTH) {
            physics_.velocity.y = 0;
            curDirection = WEST;
        }
        else if (curDirection == EAST) {
            physics_.velocity.x = 0;
            curDirection = NORTH;
        }
        break;
    case RAIL_SW:
        if (curDirection == WEST) {
            physics_.velocity.x = 0;
            curDirection = NORTH;
        }
        else if (curDirection == SOUTH) {
            physics_.velocity.y = 0;
            curDirection = EAST;
        }
        break;
    case RAIL_NW:
        if (curDirection == NORTH) {
            physics_.velocity.y = 0;
            curDirection = EAST;
        }
        else if (curDirection == WEST) {
            physics_.velocity.x = 0;
            curDirection = SOUTH;
        }
        break;
    default:
        physics_.velocity.x = 0;
        physics_.velocity.y = 0;
        break;
    }
}

void Cart::updateVelocity()
{

    int directionMultiplier = getDirectionMultiplier(curDirection);
    switch (curDirection) {
    case WEST:
        orientation = CART_H;
        physics_.velocity.x += directionMultiplier * physics_.acceleration * GetFrameTime();
        break;
    case EAST:
        orientation = CART_H;
        physics_.velocity.x += directionMultiplier * physics_.acceleration * GetFrameTime();
        break;
    case NORTH:
        orientation = CART_V;
        physics_.velocity.y += directionMultiplier * physics_.acceleration * GetFrameTime();
        break;
    case SOUTH:
        orientation = CART_V;
        physics_.velocity.y += directionMultiplier * physics_.acceleration * GetFrameTime();
        break;
    default:
        physics_.velocity.x = 0;
        physics_.velocity.y = 0;
        break;
    }
}

void EntityManager::updateCart(Cart &cart, TileManager &tileManager)
{

    Vector2 farSideAABB = getFarSideCartAABB(cart);
    /* Vector2 nearSideAABB = getNearSideCartAABB(cart); */
    int itemUnder = tileManager.getItemUnder(farSideAABB);
    cart.updateDirection(itemUnder);
    cart.updateVelocity();
}

void EntityManager::getPlayerInteraction(Player &player)
{

    if (player.state_.curAction != NORMAL) {
        int curPlayerItem = player.inventory_.itemHotbar[player.inventory_.curHotbarItem];
        switch (player.state_.curAction) {
        case ENTITY_CREATE:
            if (curPlayerItem == CART) {
                Vector2 mouseGridPos = getMouseGridPosition(player.camera_.cam);
                createCart(mouseGridPos, carts.size());
            }
            break;
        default:
            break;
        }
    }
}

void EntityManager::update(Atlas &atlas, TileManager &tileManager, Player &player)
{
    getPlayerInteraction(player);
    for (int i = 0; i < carts.size(); i++) {
        Vector2 cartPos = carts[i].physics_.pos;
        updateCart(carts[i], tileManager);
        carts[i].physics_.update();
    }
    drawCarts(atlas);
}
