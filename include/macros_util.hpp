#pragma once
#include <cassert>
#include <iostream>
#include <math.h>
#include <stdio.h> /* printf, scanf, puts, NULL */
#include <string>
#include <time.h> /* time */
#include <vector>

/* Sprite Atlas Info */
#define SPRITE_PADDING 5

/* Window info*/
#define WINDOW_WIDTH 864
#define WINDOW_HEIGHT 576

/* Game World Info*/
#define WORLD_SIZE 11
#define CHUNK_SIZE 32
#define RENDER_DISTANCE 3 // the amount of chunks near the player the tile manager will render

/* Player Info */
#define NUM_INVENTORY 50
#define NUM_HOTBAR 6

/* Entity Object Info */
#define ENTITY_STORAGE_SIZE 50

/* UI Info */
#define UI_FONT_SIZE 18

/* Gamepad Info */
#define XBOX360_LEGACY_NAME_ID "Xbox Controller"
#define XBOX360_NAME_ID "Xbox 360 Controller"
#define PS3_NAME_ID "PLAYSTATION(R)3 Controller"
