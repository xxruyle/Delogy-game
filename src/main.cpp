#include "macros_util.hpp"
#define RAYGUI_IMPLEMENTATION

#include "dev_util.hpp"
#include "player.hpp"
#include "player_camera.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include "ui.hpp"

int main()
{
    srand(time(NULL));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Delogy Indev");

    SetTargetFPS(60);
    Atlas atlas("res/default_atlas.png");
    /* PlayerCamera camera; */

    Player player(Vector2{0, 0}, Rectangle{80, 0}, 4);

    TileManager tileManager(GetRandomValue(0, 3000));
    tileManager.generateChunks();

    UI userInterface;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(player.camera_.cam);

        /* Handle Tile Manager */
        tileManager.update(atlas, player);

        /* Draw Entities */
        player.update(atlas);
        EndMode2D();

        /* Draw UI */
        drawGameInfo();
        drawMouseGridPosition(player.camera_.cam);
        drawMouseChunkPosition(player.camera_.cam);

        userInterface.showHotBar(atlas, player.inventory_);
        EndDrawing();
    }
    CloseWindow();
}
