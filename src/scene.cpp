#include "scene.hpp"
#include "components.hpp"
#include "raylib.h"

void Scene::setPlayerFocus()
{

    PositionC &p1 = EntityRegistry.get<PositionC>(player);
    playerPosition = p1.pos;
    camera.target = p1.pos;
}

void Scene::updateCamera()
{
    camera.target = Vector2Add(camera.target, {8.0f, 8.0f}); // center target

    // update camera zoom
    camera.zoom += GetMouseWheelMove();
    if (camera.zoom > 7.0f)
        camera.zoom = 7.0f;

    if (camera.zoom < 1.0f)
        camera.zoom = 1.0f;

    windowWidth = GetScreenWidth();
    windowHeight = GetScreenHeight();
    camera.offset = Vector2{round((float)(windowWidth / 2)), round((float)(windowHeight / 2))};
}

void Scene::addPlayer(Vector2 spawnPos, Rectangle frameSrc, int numFrames)
{
    entt::entity entity = EntityRegistry.create();
    player = entity;

    EntityRegistry.emplace<PlayerC>(entity, 0);
    EntityRegistry.emplace<SpriteC>(entity, frameSrc);
    EntityRegistry.emplace<PositionC>(entity, spawnPos);
    EntityRegistry.emplace<AnimationC>(entity, frameSrc, (unsigned int)numFrames);
    EntityRegistry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 100, 100, Rectangle{0, 0, 16, 16}, false);
    EntityRegistry.emplace<InventoryC>(entity);
}
