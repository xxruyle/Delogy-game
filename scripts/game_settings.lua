ANIMATION_SPEED = 15.0
ATLAS_SPRITE_PADDING = 5.0
ATLAS_SPRITE_MARGIN = 4.0

directionState = {
    WEST = "WEST",
    EAST = "EAST",
    NORTH = "NORTH",
    SOUTH = "SOUTH"
}

emotionState = {
    SAD = 0,
    HAPPY = 1
}

AtlasType = {
    SMALL = "SMALL",
    MEDIUM = "MEDIUM"

}



-- Window Info
WINDOW_WIDTH = 1260
WINDOW_HEIGHT = 780

-- Game World Info
WORLD_SIZE = 40
TILE_SIZE = 16
RENDER_DISTANCE = 2 -- the amount of chunks near the player the tile manager will render
-- WORLD_SEED = 649
WORLD_SEED = math.random(0, 1000)

-- MINI Map Info
MINIMAP_WIDTH = 200
MINIMAP_HEIGHT = 200

-- Player Info
PLAYER_SPEED = 100
PLAYER_SPRITE_SIZE = 32
NUM_INVENTORY = 50
NUM_HOTBAR = 6
COLLISION_ENABLED = false

-- NPC Info
MAX_NPCS = 500
DRAW_NPC_DEBUG = true -- draw npc info (e.g path arrows)
NPC_SPAWN_RADIUS = 50

-- UI Info
UI_FONT_SIZE = 18
