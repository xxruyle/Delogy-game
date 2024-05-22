#include "scene.hpp"
#include "animation_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"
#include "lua/lualoader.hpp"

void Scene::setPlayerFocus()
{

    PositionC& p1 = EntityRegistry.get<PositionC>(player);
    playerPosition = p1.pos;
    curTarget = Vector2Add(playerPosition, Vector2{16.0f, 16.0f});
}

void Scene::updateCamera(UI& ui)
{

    setPlayerFocus();
    camera.target = curTarget;

    // update camera zoom
    if (ui.mouseOutOfBounds()) {
        // Calculate the target zoom level based on mouse wheel input
        float newZoom = camera.zoom + GetMouseWheelMove() * 0.4f; // Adjust sensitivity if needed
        if (abs(camera.zoom - newZoom) > 0.0f) {
            targetZoom = newZoom;
            isZooming = true;
        }

        if (isZooming) {
            camera.zoom = Lerp(camera.zoom, targetZoom, 0.04f);
            if (abs(camera.zoom - targetZoom) < 0.001f) {
                isZooming = false;
            }

            // zoom in: 10.0f
            // zoom out limit: 0.05f
            // debug mode
            camera.zoom = Clamp(camera.zoom, 0.05f, 10.0f);

            // actual game mode
            /* camera.zoom = Clamp(camera.zoom, 1.3f, 6.0f); */
        }
    }

    windowWidth = GetScreenWidth();
    windowHeight = GetScreenHeight();
    camera.offset = Vector2{round((float)(windowWidth / 2)), round((float)(windowHeight / 2))};
}

void Scene::addPlayer(AtlasType atlasid, Vector2 spawnPos, Rectangle frameSrc, int numFrames, int framesPerRow)
{
    entt::entity entity = EntityRegistry.create();
    player = entity;

    EntityRegistry.emplace<PlayerC>(entity, 0);
    EntityRegistry.emplace<SpriteC>(entity, atlasid, frameSrc);
    EntityRegistry.emplace<PositionC>(entity, spawnPos);
    EntityRegistry.emplace<AnimationC>(entity, frameSrc, (unsigned int)numFrames, (unsigned int)framesPerRow);

    int playerSpeed = LuaGetInt("PLAYER_SPEED", "scripts/game_settings.lua");
    EntityRegistry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, playerSpeed, false);

    float ATLAS_SPRITE_PADDING = LuaGetInt("ATLAS_SPRITE_PADDING", "scripts/game_settings.lua");
    float ATLAS_SPRITE_MARGIN = LuaGetInt("ATLAS_SPRITE_MARGIN", "scripts/game_settings.lua");

    EntityRegistry.emplace<CollisionC>(entity, Rectangle{15 - ATLAS_SPRITE_MARGIN, 29 - ATLAS_SPRITE_MARGIN, 11, 7});
    EntityRegistry.emplace<InventoryC>(entity);
}
