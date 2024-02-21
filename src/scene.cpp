#include "scene.hpp"
#include "animation_system.hpp"
#include "components.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"

void Scene::setPlayerFocus()
{

    PositionC &p1 = EntityRegistry.get<PositionC>(player);
    playerPosition = p1.pos;
    curTarget = Vector2Add(playerPosition, Vector2{16.0f, 16.0f});
}

void Scene::updateCamera()
{

    /* Vector2 lerpTarget = Vector2Lerp(prevCameraTarget, curTarget, 0.05f); */
    /* prevCameraTarget = lerpTarget; */
    /* camera.target = Vector2Add(lerpTarget, Vector2{16.0f, 16.0f}); */
    /* curTarget = camera.target; */

    camera.target = curTarget;

    // update camera zoom
    camera.zoom += GetMouseWheelMove() * 0.5f;
    if (camera.zoom > 10.0f) // zoom in limit
        camera.zoom = 10.0f;

    if (camera.zoom < 0.05f) // zoom out limit
        camera.zoom = 0.05f;

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

    EntityRegistry.emplace<DirectionStateC>(entity, (int)directionState::SOUTH);

    EntityRegistry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 80, 80, false);
    EntityRegistry.emplace<CollisionC>(entity, Rectangle{15 - ATLAS_SPRITE_MARGIN, 29 - ATLAS_SPRITE_MARGIN, 11, 7});
    EntityRegistry.emplace<InventoryC>(entity);
}
