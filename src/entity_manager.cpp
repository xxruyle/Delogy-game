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
        if (cart.curDirection == SOUTH) {
            cart.curDirection = EAST;
        }
        else if (cart.curDirection == NORTH) {
            cart.curDirection = WEST;
        }
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
        if (cart.curDirection == EAST) {
            cart.curDirection = SOUTH;
        }
        else if (cart.curDirection == WEST) {
            cart.curDirection = NORTH;
        }
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
        cart.orientation = CART_H;
        cart.physics_.velocity.x += directionMultiplier * cart.physics_.acceleration * GetFrameTime();
        break;
    case EAST:
        cart.orientation = CART_H;
        cart.physics_.velocity.x += directionMultiplier * cart.physics_.acceleration * GetFrameTime();
        break;
    case NORTH:
        cart.orientation = CART_V;
        cart.physics_.velocity.y += directionMultiplier * cart.physics_.acceleration * GetFrameTime();
        break;
    case SOUTH:
        cart.orientation = CART_V;
        cart.physics_.velocity.y += directionMultiplier * cart.physics_.acceleration * GetFrameTime();
        break;
    default:
        cart.physics_.velocity.x = 0;
        cart.physics_.velocity.y = 0;
        break;
    }
}

void EntityManager::getPlayerInteraction(Player &player)
{
    int curPlayerItem = player.inventory_.itemHotbar[player.inventory_.curHotbarItem];
    if (player.state_.curState == INTERACTING) {
        switch (player.state_.curAction) {
        case ENTITY_CREATE:
            if (curPlayerItem == CART_H_ || curPlayerItem == CART_V_) {
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
