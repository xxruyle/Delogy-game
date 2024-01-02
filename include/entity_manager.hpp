#include "atlas.hpp"
#include "dev_util.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"

enum cartState { MOVE, STOPPED };
enum cartDirection { WEST = 0, EAST = 1, NORTH = 2, SOUTH = 3 };

int getDirectionMultiplier(int direction);

struct EntityStorage {};

class EntityPhysics {
  public:
    Vector2 pos;
    EntityPhysics(Vector2 gridPos);
    Vector2 velocity = {0, 0};
    float acceleration = 5.0f;
    float maxSpeed = 100.0f;
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
    void update();
};

class EntityManager {
  public:
    std::vector<Cart> carts;
    void getCartPaths(TileManager &tileManager);
    void updateCart(Cart &cart, TileManager &tileManager); // changes cart direction based on rail underneath it
    void getPlayerInteraction(Player &player);
    void update(Atlas &atlas, TileManager &tileManager, Player &player);
    void createCart(Vector2 gridPos, int id);
    void populateCarts();
    void drawCarts(Atlas &atlas);
};
