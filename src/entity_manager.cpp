#include "entity_manager.hpp"
#include "dev_util.hpp"
#include "entity_data.hpp"
#include "item_data.hpp"
#include "raylib.h"
#include "player.hpp"
#include <string>

std::unordered_set<int> getValidRails(int rail, int direction)
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

EntityPhysics::EntityPhysics(Vector2 gridPos)
{
    prevGridPos = gridPos;
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

Vector2 getFarSideCartBorder(Cart &cart)
{
    Vector2 cartPos;
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
    return cartPos;
}

Vector2 getNearSideCartBorder(Cart &cart)
{
    Vector2 cartPos;
    switch (cart.curDirection) {
    case WEST:
        cartPos = {cart.physics_.pos.x + 1.0f, cart.physics_.pos.y + 8.0f};
        break;
    case EAST:
        cartPos = {cart.physics_.pos.x + 15.0f, cart.physics_.pos.y + 8.0f};
        break;
    case NORTH:
        cartPos = {cart.physics_.pos.x + 8.0f, cart.physics_.pos.y};
        break;
    case SOUTH:
        cartPos = {cart.physics_.pos.x, cart.physics_.pos.y + 15.0f};
        break;
    default:
        cartPos = cart.physics_.pos;
        break;
    }
    return cartPos;
}

void Cart::updateDirection(int itemUnder, int prevItemUnder, int futureItemUnder)
{
    Vector2 curGridPos = getGridPosition({physics_.pos.x + 8.0f, physics_.pos.y + 8.0f});

    if (prevItemUnder != NULL_ITEM && futureItemUnder != NULL_ITEM) {

        if (physics_.prevGridPos.x != curGridPos.x || physics_.prevGridPos.y != curGridPos.y ||
            curState == DERAILED) { // recheck if the cart is stopped

            std::unordered_set<int> connectableRails = getValidRails(
                prevItemUnder, curDirection); // get all the connectable rails for the previous rail the cart was under

            if (!connectableRails.count(
                    futureItemUnder)) { // if the future rail that the cart is going on is connectable
                curState = DERAILED;
                physics_.velocity.x = 0;
                physics_.velocity.y = 0;
            }
            else {
                curState = MOVE;
            }
        }
    }

    /* std::cout << curState << " " << orientation << " | Previous: " << prevItemUnder << " | Future: " <<
     * futureItemUnder */
    /*           << std::endl; */

    if (curState == MOVE) {
        physics_.prevGridPos = curGridPos;
    }

    switch (itemUnder) {
    case RAIL_H:
        if (curDirection == SOUTH) {
            curDirection = EAST;
        }
        else if (curDirection == NORTH) {
            curDirection = WEST;
        }
        break;
        previousRail = RAIL_H;
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
        previousRail = RAIL_NE;
    case RAIL_V:
        if (curDirection == EAST) {
            curDirection = SOUTH;
        }
        else if (curDirection == WEST) {
            curDirection = NORTH;
        }
        break;
        previousRail = RAIL_V;
    case RAIL_SE:
        if (curDirection == SOUTH) {
            physics_.velocity.y = 0;
            curDirection = WEST;
        }
        else if (curDirection == EAST) {
            physics_.velocity.x = 0;
            curDirection = NORTH;
        }
        previousRail = RAIL_SE;
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
        previousRail = RAIL_SW;
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
        previousRail = RAIL_NW;
        break;
    default: // if the item underneat is the NULL_ITEM
        curState = DERAILED;
        physics_.velocity.x = 0;
        physics_.velocity.y = 0;
        break;
    }
}

void Cart::updateVelocity()
{

    int directionMultiplier = getDirectionMultiplier(curDirection);

    if (curState == MOVE) {
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
        }
    }
    else {
        // change orientation to derailed sprites
        if (orientation == CART_H) {
            orientation = CART_HD;
        }
        else if (orientation == CART_V) {
            orientation = CART_VD;
        }

        physics_.velocity.x = 0;
        physics_.velocity.y = 0;
    }
    /* std::cout << curState << " | Direction: " << curDirection << " | speed: " << physics_.velocity.x << " " */
    /*           << physics_.velocity.y << std::endl; */
}

void EntityManager::updateCart(Cart &cart, TileManager &tileManager)
{
    Vector2 farSideAABB = getFarSideCartBorder(cart);
    int itemUnder = tileManager.getItemUnder(farSideAABB);

    int prevItemUnder =
        tileManager.getItemUnder({cart.physics_.prevGridPos.x * 16.0f, cart.physics_.prevGridPos.y * 16.0f});

    int futureItemUnder = tileManager.getItemUnder({cart.physics_.pos.x + 8.0f, cart.physics_.pos.y + 8.0f});

    cart.updateDirection(itemUnder, prevItemUnder, futureItemUnder);
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
    derails = 0;
    for (int i = 0; i < carts.size(); i++) {
        Vector2 cartPos = carts[i].physics_.pos;
        updateCart(carts[i], tileManager);
        carts[i].physics_.update();
        if (carts[i].curState == DERAILED) {
            derails += 1;
        }
    }
    drawCarts(atlas);
}

void showEntityInfo(EntityManager &entityManager)
{
    std::string entityCountStr = "Entity Count: " + std::to_string(entityManager.carts.size());
    std::string derailmentsStr = "Entity Derails: " + std::to_string(entityManager.derails);

    DrawText(entityCountStr.c_str(), 2, 175, UI_FONT_SIZE, RAYWHITE);
    DrawText(derailmentsStr.c_str(), 2, 200, UI_FONT_SIZE, RAYWHITE);
}
