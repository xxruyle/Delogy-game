#pragma once

/* Animation Info */
#define ANIMATION_SPEED 15.0f
#define ATLAS_SPRITE_PADDING 5.0f
#define ATLAS_SPRITE_MARGIN 4.0f

enum class directionState { WEST, EAST, NORTH, SOUTH };
enum class emotionState { SAD = 0, HAPPY };
enum AtlasType { SMALL, MEDIUM };

/* Window info*/
#define WINDOW_WIDTH 864
#define WINDOW_HEIGHT 576

/* Game World Info*/
#define WORLD_SIZE 10
#define CHUNK_SIZE 32
#define RENDER_DISTANCE                                                        \
  10 // the amount of chunks near the player the tile manager will render

/* Player Info */
#define PLAYER_SPRITE_SIZE 32
#define NUM_INVENTORY 50
#define NUM_HOTBAR 6

/* Entity Object Info */
#define ENTITY_STORAGE_SIZE 50
#define ORECART_STORAGE_SIZE 20

/* UI Info */
#define UI_FONT_SIZE 18
