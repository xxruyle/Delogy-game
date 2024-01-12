#include "atlas.hpp"
#include "components.hpp"
#include "entt/entity/registry.hpp"
#include "input_system.hpp"
#include "item_data.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "tile_manager.hpp"
#include <unordered_set>

enum cartState { MOVE, DERAILED };
enum cartDirection { WEST = 0, EAST = 1, NORTH = 2, SOUTH = 3 };

class CartManager {
  public:
    void createCart(Vector2 position, InputSystem &input, entt::basic_registry<> &registry);
    void changeCartDirection(PositionC &position, OrecartC &orecart, PhysicsC &physics, int itemUnder,
                             int prevItemUnder, int futureItemUnder);
    void changeCartVelocity(PhysicsC &physics, OrecartC &orecart);
    void changeCartPosition(PositionC &position, PhysicsC &physics);
    void updateCarts(entt::basic_registry<> &registry, TileManager &tileManager);
    void getPlayerInteraction(InputSystem &input, InventoryC &inventory, Camera2D &camera,
                              entt::basic_registry<> &registry);
    void update(TileManager &tileManager, InputSystem &input, Scene &scene);

  private:
    int getDirectionMultiplier(int direction);
    std::unordered_set<int> getValidRails(
        int rail,
        int direction); // given a current rail return all the valid future rail connections depending on cart direction

    Vector2
    getFarSideCartBorder(PositionC &position,
                         int direction); // e.g if car is going EAST then it returns the left side of the cart's BB
    Vector2
    getNearSideCartBorder(PositionC &position,
                          int direction); // e.g if car is going EAST then it returns the right side of the cart's BB
};
