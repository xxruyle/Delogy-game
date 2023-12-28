#include "macros_util.hpp"
#define RAYGUI_IMPLEMENTATION

#include "dev_util.hpp"
#include "player.hpp"
#include "player_camera.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include "user_interface.h"

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

        UIRowGridRec({20, (float)(GetScreenHeight() - 100), 40, 40}, 1.2f, 1.9f, 10, RAYWHITE,
                     Color{255, 255, 255, 32});
        EndDrawing();
    }
    CloseWindow();
}
