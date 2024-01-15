#include "scene.hpp"
#include "components.hpp"
#include "macros_util.hpp"
#include "raylib.h"

void Scene::setPlayerFocus()
{

    PositionC &p1 = EntityRegistry.get<PositionC>(player);
    playerPosition = p1.pos;
    camera.target = p1.pos;
}

void Scene::updateCamera()
{
    camera.target = Vector2Add(camera.target, {12.0f, 12.0f}); // center target

    // update camera zoom
    camera.zoom += GetMouseWheelMove();
    if (camera.zoom > 10.0f)
        camera.zoom = 10.0f;

    if (camera.zoom < 0.3f)
        camera.zoom = 0.3f;

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
    EntityRegistry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 100, 100, false);
    EntityRegistry.emplace<CollisionC>(entity, Rectangle{15 - ATLAS_SPRITE_MARGIN, 29 - ATLAS_SPRITE_MARGIN, 11, 7});
    EntityRegistry.emplace<InventoryC>(entity);
}
