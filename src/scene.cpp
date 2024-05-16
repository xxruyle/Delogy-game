#include "scene.hpp"
#include "animation_system.hpp"
#include "components.hpp"
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
        camera.zoom += GetMouseWheelMove() * 0.5f;
        if (camera.zoom > 10.0f) // zoom in limit
            camera.zoom = 10.0f;

        if (camera.zoom < 0.05f) // zoom out limit
            camera.zoom = 0.05f;
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

    EntityRegistry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 70, 70, false, true);

    float ATLAS_SPRITE_PADDING = LuaGetInt("ATLAS_SPRITE_PADDING", "scripts/game_settings.lua");
    float ATLAS_SPRITE_MARGIN = LuaGetInt("ATLAS_SPRITE_MARGIN", "scripts/game_settings.lua");

    EntityRegistry.emplace<CollisionC>(entity, Rectangle{15 - ATLAS_SPRITE_MARGIN, 29 - ATLAS_SPRITE_MARGIN, 11, 7});
    EntityRegistry.emplace<InventoryC>(entity);
}
