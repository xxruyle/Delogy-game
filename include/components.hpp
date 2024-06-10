#pragma once
#include "atlas.hpp"
#include "atlas_data.hpp"
#include "dev_util.hpp"
#include "lua/lualoader.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include <array>
#include <cstddef>
#include <deque>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>

struct SpriteC {
  AtlasType atlasID; // the atlas the sprite is located on
  Rectangle atlasPos;
};

struct PositionC {
  Vector2 pos;
};

struct CollisionC {
  Rectangle aabb;
  Vector2 normal;
};

struct PhysicsC {
  Vector2 velocity;
  int speed;
  bool is_solid;

  bool moving() { return velocity.x != 0 || velocity.y != 0; };

  void stop() {
    velocity.x = 0;
    velocity.y = 0;
  };
};

struct AnimationC {
  Rectangle frameOrigin;     // the top left of the animation set in the atlas
  unsigned int numFrames;    // the number of different animation states
  unsigned int framesPerRow; // the number of animations per state
  int curFrameSrc; // the index of the cur frame we will draw from frameSrcs
                   // (the animation state)
  std::vector<Rectangle> frameSrcs; // the src of each frame

  /* Example: If our curFrameSrc is 0, we will just multiply by atFrame[0] *
   * 16
   * * padding to get to that frame  */
  std::vector<unsigned int> atFrame; // the number each animation state is on

  float animationSpeed = Slua::lua["ANIMATION_SPEED"].get_or(0.0f);

  float ATLAS_SPRITE_PADDING = Slua::lua["ATLAS_SPRITE_PADDING"].get_or(0.0f);

  float animationUpdateTime = 1.0f / animationSpeed;
  float timeSinceLastFrameSwap = 0.0f;

  AnimationC(Rectangle origin, unsigned int numframes,
             unsigned int numFramesPerRow)
      : frameOrigin(origin), numFrames(numframes),
        framesPerRow(numFramesPerRow) {
    curFrameSrc = 3;
    for (unsigned int i = 0; i < numFrames; i++) {
      frameSrcs.push_back((Rectangle{
          (float)(origin.x),
          (float)(origin.y + i * (origin.height + ATLAS_SPRITE_PADDING)),
          (float)origin.width, (float)origin.height}));
    }

    for (unsigned int i = 0; i < framesPerRow; i++) {
      atFrame.push_back(0);
    }
  };
};

struct InventoryC {
  std::vector<int> slots = {
      RAIL_V,
      CART,
      STORAGE_BOX,
      MUSHROOM_PURPLE,
  };

  std::vector<int> stacks = {1, 1, 1, 1};

  InventoryC(size_t capacity) {
    slots.resize(capacity);
    stacks.resize(capacity);
  };
};

// front end access to inventory slots
struct HotBarC {
  int capacity;
  int curItem;
};

struct ItemC {
  int id;
  int capacity; // number of items at position
};

struct UIInventoryC {
  bool active;
  Vector2 srcPos;
};

struct OrecartC {
  int orientation;
  int movementDirection;
  int previousRail;
  Vector2 previousGridPos;
};

struct PlayerC {
  int timePlayed;
};

struct PathC {
  Vector2 target;
  bool targetAvailable;          // target is ready
  std::deque<Vector2> destQueue; // the destinations in queue
  unsigned int targetID;

  void setTarget(Vector2 dest) {
    target.x = dest.x;
    target.y = dest.y;
    targetAvailable = true;
  }
};

struct TimerC {
  float lastTime = 0.0f;
};

enum needType { SATIATION, ENERGY, SOCIAL, ENTERTAINMENT, SAFETY };
enum actionType {
  EATING,
  RESTING,
  SOCIALIZING,
  PLAY,
  DEFENSE,
  SEARCHING,
  NONE
};

struct GenesC {
  float maxDesires[5];     // range rn: (70 - 100)   // the most a desire can be
  float minDesires[5];     // the minimum desire before a state must be taken
  int desireIncrements[5]; // how much a desire is incremented
  int desireDecrements[5]; // how mcuh a desire is decremeneed
};

// genes influence the needs
struct NeedsC {
  float weights[5];
  float desires[5];

  // states (graph nodes)
  int currentDesire = ENERGY;
  int currentAction = RESTING;
  int currentSubAction = NONE; // the action that the entity can do while doing
                               // something else!

  // action states (graph edges), managed by npc system
  bool leisure = false;
  bool eating = false;
  bool social = false;
  bool aroundFriends = false;
  bool search = false;
};
