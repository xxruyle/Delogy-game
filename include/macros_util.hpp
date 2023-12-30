#pragma once
#include <cassert>
#include <iostream>
#include <math.h>
#include <stdio.h> /* printf, scanf, puts, NULL */
#include <string>
#include <time.h> /* time */
#include <vector>

/* Window info*/
#define WINDOW_WIDTH 864
#define WINDOW_HEIGHT 576

/* Game World Info*/
#define WORLD_SIZE 10
#define CHUNK_SIZE 32
#define RENDER_DISTANCE 3 // the amount of chunks near the player the tile manager will render

/* Player Info */
#define NUM_INVENTORY 50
#define NUM_HOTBAR 6
