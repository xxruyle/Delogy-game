#include "entity_manager.hpp"
#include "entity_data.hpp"
#include "raylib.h"

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

void EntityManager::populateCarts()
{
    for (int i = 0; i < 1; i++) {
        EntityStorage tempStorage;
        EntityPhysics tempPhysics;
        Cart tempCart = {i, tempPhysics, tempStorage};
        carts.push_back(tempCart);
    }
}

void EntityManager::drawCarts(Atlas &atlas)
{
    int cartID = CART_H; // JUST A placeholder for now for ENTITY_CART_H
    for (int i = 0; i < carts.size(); i++) {
        Entity curEntity = entityids[cartID];
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

void EntityManager::updateCart(Cart &cart, TileManager &tileManager)
{
    Vector2 cartPos;
    // change the cart pos to check depending on the direction of the cart
    switch (cart.curDirection) {
    case WEST:
        cartPos = {cart.physics_.pos.x + 15.0f, cart.physics_.pos.y + 8.0f}; // put position on the right side
        break;
    case EAST:
        cartPos = {cart.physics_.pos.x, cart.physics_.pos.y + 8.0f};
        break;
    case NORTH:
        cartPos = {cart.physics_.pos.x + 8.0f, cart.physics_.pos.y + 15.0f}; // put position on the right side
        break;
    case SOUTH:
        cartPos = {cart.physics_.pos.x, cart.physics_.pos.y}; // put position on the right side
        break;
    default:
        cartPos = cart.physics_.pos;
        break;
    }

    int itemUnder = tileManager.getItemUnder(cartPos);

    switch (itemUnder) {
    case RAIL_H:
        break;
    case RAIL_NE:
        if (cart.curDirection == EAST) {
            cart.physics_.velocity.x = 0;
            cart.curDirection = SOUTH;
        }
        else if (cart.curDirection == NORTH) {
            cart.physics_.velocity.y = 0;
            cart.curDirection = WEST;
        }
        break;
    case RAIL_V:
        break;
    case RAIL_SE:
        if (cart.curDirection == SOUTH) {
            cart.physics_.velocity.y = 0;
            cart.curDirection = WEST;
        }
        else if (cart.curDirection == EAST) {
            cart.physics_.velocity.x = 0;
            cart.curDirection = NORTH;
        }
        break;
    case RAIL_SW:
        if (cart.curDirection == WEST) {
            cart.physics_.velocity.x = 0;
            cart.curDirection = NORTH;
        }
        else if (cart.curDirection == SOUTH) {
            cart.physics_.velocity.y = 0;
            cart.curDirection = EAST;
        }
        break;
    case RAIL_NW:
        if (cart.curDirection == NORTH) {
            cart.physics_.velocity.y = 0;
            cart.curDirection = EAST;
        }
        else if (cart.curDirection == WEST) {
            cart.physics_.velocity.x = 0;
            cart.curDirection = SOUTH;
        }
        break;
    default:
        cart.physics_.velocity.x = 0;
        cart.physics_.velocity.y = 0;
        break;
    }

    int directionMultiplier = getDirectionMultiplier(cart.curDirection);
    switch (cart.curDirection) {
    case WEST:
        cart.physics_.velocity.x += directionMultiplier * cart.physics_.acceleration * GetFrameTime();
        break;
    case EAST:
        cart.physics_.velocity.x += directionMultiplier * cart.physics_.acceleration * GetFrameTime();
        break;
    case NORTH:
        cart.physics_.velocity.y += directionMultiplier * cart.physics_.acceleration * GetFrameTime();
        break;
    case SOUTH:
        cart.physics_.velocity.y += directionMultiplier * cart.physics_.acceleration * GetFrameTime();
        break;
    default:
        cart.physics_.velocity.x = 0;
        cart.physics_.velocity.y = 0;
        break;
    }
}

void EntityManager::update(Atlas &atlas, TileManager &tileManager)
{
    for (int i = 0; i < carts.size(); i++) {
        Vector2 cartPos = carts[i].physics_.pos;
        updateCart(carts[i], tileManager);
        carts[i].physics_.update();
    }
    drawCarts(atlas);
}
