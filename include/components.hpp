#pragma once
#include "item_data.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include <vector>

#define ANIMATION_SPEED 5.0f
#define ATLAS_SPRITE_PADDING 5.0f
#define ORECART_STORAGE_SIZE 20

enum class directionState { WEST, EAST, NORTH, SOUTH };

struct DirectionStateC {};

struct SpriteC {
    Rectangle atlasPos;
};

struct PositionC {
    Vector2 pos;
};

struct PhysicsC {
    Vector2 velocity;
    unsigned int speed;
    unsigned int maxSpeed;
    Rectangle aabb;
    bool moving;
};

struct AnimationC {
    Rectangle frameOrigin; // the top left of the animation set in the atlas
    unsigned int numFrames;
    int curFrameSrc;                  // the index of the cur frame we will draw from frameSrcs
    std::vector<Rectangle> frameSrcs; // the src of each frame

    /* Example: If our curFrameSrc is 0,
    we will just multiply by atFrame[0] * 16 * padding to get to that frame  */
    std::vector<unsigned int> atFrame; // the number each frame is on

    float animationSpeed = ANIMATION_SPEED;
    float animationUpdateTime = 1.0f / ANIMATION_SPEED;
    float timeSinceLastFrameSwap = 0.0f;

    AnimationC(Rectangle origin, unsigned int numframes) : frameOrigin(origin), numFrames(numframes) {
        curFrameSrc = 3;
        for (unsigned int i = 0; i < numFrames; i++) {
            frameSrcs.push_back((Rectangle{(float)(origin.x), (float)(origin.y + i * (16 + ATLAS_SPRITE_PADDING)),
                                           (float)16, (float)16}));
        }

        for (unsigned int i = 0; i < numFrames; i++) {
            atFrame.push_back(0);
        }
    };
};

struct InventoryC {
    int hotbar[NUM_HOTBAR] = {RAIL_V, CART, NULL_ITEM, NULL_ITEM, NULL_ITEM};
    int curItem = 0;
};

struct OrecartC {
    int storage[ORECART_STORAGE_SIZE];
};

struct PlayerC {
    int timePlayed;
};
