#pragma once
#include "atlas.hpp"
#include "entity_data.hpp"
#include "item_data.hpp"
#include "macros_util.hpp"
#include "player_camera.hpp"
#include "raylib.h"

enum animationDir { RIGHT, LEFT, UP, DOWN };
enum stateType { MOVING, IDLE, INTERACTING };
enum action {
    NORMAL,
    ITEM_CREATE,
    DESTROY,
    ENTITY_CREATE
}; // whether the player is performing normal actions or interactive actions

struct PlayerState {
    enum stateType curState = IDLE;
    enum action curAction = NORMAL;
};

class PlayerPhysics {
  public:
    Vector2 pos;
    Vector2 dir = {0, 0};
    Vector2 velocity = {0, 0};
    float acceleration = 700.0f;
    float decel = 700.0f;
    float maxSpeed = 120.0f;

    PlayerPhysics(Vector2 spawnPos) : pos(spawnPos){};
    void clampSpeed();
    void update();
};

class PlayerAnimation {
  public:
    enum animationDir curAnimation = DOWN;

    Rectangle curRec;           // the cur frame that will be drawn
    Rectangle animationSrcs[4]; // right, left, up, down
    int curFrames[4];           // the curFrames of each animation rec
    int frameAmount;

    float animationSpeed = 5.0f;
    float animationUpdateTime = 1.0f / animationSpeed;
    float timeSinceLastFrameSwap = 0.0f;

    PlayerAnimation(Rectangle src, int animationFrames);

    void update(PlayerState &state);
    void changeAnimation(PlayerState &state); // increment the current animation's rectangle along the atlas
};

class PlayerInventory {
  public:
    /* int itemInventory[NUM_INVENTORY]; */
    int itemHotbar[NUM_HOTBAR] = {RAIL_V, CART, NULL_ITEM, NULL_ITEM, NULL_ITEM};
    int curHotbarItem = 0; // the cur item that the player is selecting
};

class PlayerInput {
  public:
    bool firstPressed = false;
    int curGamepad = 0;

    // Gamepad axis
    Vector2 gamepadDir;
    Vector2 rightStickAxis;

    bool isGamepadBeingUsed(PlayerState &state);
    void getGamePadInput(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state);
    void getGamePadMouseInput();
    void getGamePadAnimation(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state);
    void resetGamePadInput(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state);

    // Keyboard input
    void getInput(PlayerPhysics &physics, PlayerAnimation &animation,
                  PlayerState &state);                  // changes player velocity based on player input
    void getAnimationInput(PlayerAnimation &animation); // changes player animation based on player input
    void resetInput(PlayerAnimation &animation, PlayerState &state, PlayerPhysics &physics);

    void getInteractState(PlayerCamera &camera, PlayerState &state,
                          PlayerInventory &inventory); // TODO ask for user input in here
    void getInventoryChoice(PlayerInventory &inventory);
    void getItemRotation(PlayerInventory &inventory);

    void update(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state, PlayerCamera &camera,
                PlayerInventory &inventory);
};

class Player {
  public:
    Player(Vector2 spawnPos, Rectangle src, int animationFrames);

    // TODO move all the components to other header and src files!  (like PlayerCamera)
    // worried that might create circular dependencies thoughbeit
    PlayerAnimation animation_;
    PlayerPhysics physics_;
    PlayerInput input_;
    PlayerState state_;
    PlayerCamera camera_;
    PlayerInventory inventory_;

    Rectangle boundingRec = Rectangle{1, 0, 16, 17}; // player bounding box

    void update(Atlas &atlas);
};
