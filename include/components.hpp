#pragma once
#include "atlas.hpp"
#include "dev_util.hpp"
#include "item_data.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include <deque>
#include <queue>
#include <unordered_set>
#include <vector>

struct DirectionStateC {
  int curDirection;
};

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
  int maxSpeed;
  bool moving;
  bool is_solid;
};

struct AnimationC {
  Rectangle frameOrigin;     // the top left of the animation set in the atlas
  unsigned int numFrames;    // the number of different animation states
  unsigned int framesPerRow; // the number of animations per state
  int curFrameSrc; // the index of the cur frame we will draw from frameSrcs
                   // (the animation state)
  std::vector<Rectangle> frameSrcs; // the src of each frame

  /* Example: If our curFrameSrc is 0, we will just multiply by atFrame[0] * 16
   * * padding to get to that frame  */
  std::vector<unsigned int> atFrame; // the number each animation state is on

  float animationSpeed = ANIMATION_SPEED;
  float animationUpdateTime = 1.0f / ANIMATION_SPEED;
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
  int hotbar[NUM_HOTBAR] = {RAIL_V, CART, STORAGE_BOX, NULL_ITEM, NULL_ITEM};
  int curItem = 0;
};

struct OrecartC {
  int orientation;
  int movementDirection;
  int previousRail;
  Vector2 previousGridPos;
  int storage[ORECART_STORAGE_SIZE];
};

struct PlayerC {
  int timePlayed;
};

struct PathC {
  Vector2 target;
  bool isPathing;
  bool atTarget;
  std::deque<Vector2> destQueue; // the destinations in queue
};

struct TimerC {
  float lastTime = 0.0f;
};

enum needType { ENERGY, HUNGER, SOCIAL, ENTERTAINMENT, SAFETY };

struct NeedsC {
  float weights[5];
  float desires[5];
};
