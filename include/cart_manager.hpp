#include "atlas.hpp"
#include "item_data.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include <unordered_set>

enum cartState { MOVE, DERAILED };
enum cartDirection { WEST = 0, EAST = 1, NORTH = 2, SOUTH = 3 };

int getDirectionMultiplier(int direction);

std::unordered_set<int> getValidRails(
    int rail,
    int direction); // given a current rail return all the valid future rail connections depending on cart direction

struct EntityStorage {};

class EntityPhysics {
  public:
    Vector2 pos;
    Vector2 prevGridPos;
    EntityPhysics(Vector2 gridPos);
    Vector2 velocity = {0, 0};
    float acceleration = 50.0f;
    float maxSpeed = 50.0f;
    void clampSpeed();
    void update();
};

class Cart {
  public:
    int id;          // the id of the individual cart
    int orientation; // vertical or horizontal
    EntityPhysics physics_;

    int storage[ENTITY_STORAGE_SIZE];
    enum cartState curState = MOVE;
    enum cartDirection curDirection = WEST;
    int previousRail = NULL_ITEM;
    void updateDirection(int itemUnder, int prevItemUnder, int futureItemUnder);
    void updateVelocity();
    void update();
};

Vector2 getFarSideCartBorder(Cart &cart);  // e.g if car is going EAST then it returns the left side of the cart's BB
Vector2 getNearSideCartBorder(Cart &cart); // e.g if car is going EAST then it returns the right side of the cart's BB

class CartManager {
  public:
    std::vector<Cart> carts;
    int derails = 0;
    void updateCart(Cart &cart, TileManager &tileManager); // changes cart direction based on rail underneath it
    /* void getPlayerInteraction(Player &player); */
    void update(Atlas &atlas, TileManager &tileManager);
    void createCart(Vector2 gridPos, int id);
    void populateCarts();
    void drawCarts(Atlas &atlas);
};
