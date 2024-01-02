#include "atlas.hpp"
#include "entity_data.hpp"
#include "item_data.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"
#include "tile_manager.hpp"

enum cartState { MOVE, STOPPED };
enum cartDirection { WEST = 0, EAST = 1, NORTH = 2, SOUTH = 3 };

int getDirectionMultiplier(int direction);

struct EntityStorage {
    int storage[ENTITY_STORAGE_SIZE];
};

class EntityPhysics {
  public:
    Vector2 pos;
    EntityPhysics() : pos({(float)GetRandomValue(0, 200), 0}){};
    Vector2 velocity = {0, 0};
    float acceleration = 5.0f;
    float maxSpeed = 100.0f;
    void clampSpeed();
    void update();
};

class Cart {
  public:
    int id;
    EntityPhysics physics_;
    EntityStorage itemStorage;
    enum cartState curState = MOVE;
    enum cartDirection curDirection = WEST;
    void update();
};

class EntityManager {
  public:
    std::vector<Cart> carts;
    void getCartPaths(TileManager &tileManager);
    void updateCart(Cart &cart, TileManager &tileManager); // changes cart direction based on rail underneath it
    void update(Atlas &atlas, TileManager &tileManager);
    void populateCarts();
    void drawCarts(Atlas &atlas);
};
