#include "npc_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include <string>

NPCSystem::NPCSystem(TileManager* tileManager) { tManager = tileManager; }

void NPCSystem::addNPCs(entt::basic_registry<>& registry)
{
    for (int i = 0; i < 10; i++) {
        entt::entity entity = registry.create();
        registry.emplace<SpriteC>(entity, AtlasType::SMALL, Rectangle{88, 4, 16, 16});
        registry.emplace<PositionC>(entity, Vector2{GetRandomValue(-100, 100), GetRandomValue(-100, 100)});
        registry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 5, 5, false);
        registry.emplace<CollisionC>(entity, Rectangle{0, 0, 16, 16});
        registry.emplace<NeedsC>(entity, NeedsC{{(float)GetRandomValue(1, 10) / 10.0f, 0.3f, 0.1f, 0.1f, 0.3f}});
    }
}

void NPCSystem::update(Scene& scene) { showEntityInfo(scene.camera, scene.EntityRegistry); }

void NPCSystem::cachePosition(Vector2 pos, entt::entity id)
{

    /* std::cout << getVector2String(pos) << std::endl; */

    tManager->entityPositionCache[pos].push_back(id);
}

void NPCSystem::moveNPCs(entt::basic_registry<>& registry)
{

    auto view = registry.view<PositionC, NeedsC, PhysicsC>();

    for (auto entity : view) {
        auto& needs = view.get<NeedsC>(entity);
        auto& position = view.get<PositionC>(entity);
        if (GetRandomValue(0, 100) > 90) {
            position.pos.x += 2 * GetFrameTime();
        }
        else {
            position.pos.x -= 2 * GetFrameTime();
        }

        Vector2 entityPos = getGridPosition(position.pos);
        cachePosition(entityPos, entity);

        auto& physics = view.get<PhysicsC>(entity);
    }
}

void NPCSystem::showEntityInfo(Camera2D& camera, entt::basic_registry<>& registry)
{
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        Vector2 pos = getMouseGridPosition(camera);

        for (int x = pos.x - 1; x < pos.x + 1; x++) {
            for (int y = pos.y - 1; y < pos.y + 1; y++) {
                Vector2 nearPos = {(float)x, (float)y};
                /* std::cout << getVector2String(nearPos) << std::endl; */
                if (tManager->entityPositionCache.count(nearPos)) {
                    for (entt::entity id : tManager->entityPositionCache[nearPos]) {
                        PositionC& position = registry.get<PositionC>(id);
                        CollisionC& coll = registry.get<CollisionC>(id);

                        Rectangle absoluteAABB = {coll.aabb.x + position.pos.x, coll.aabb.y + position.pos.y, coll.aabb.width, coll.aabb.height};

                        if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), absoluteAABB)) {
                            NeedsC& needs = registry.get<NeedsC>(id);
                            Vector2 infoPos = GetScreenToWorld2D(GetMousePosition(), camera);

                            std::string weightString = std::to_string(needs.weights[0]);
                            weightString.erase(weightString.find_last_not_of('0') + 1, std::string::npos);
                            weightString.erase(weightString.find_last_not_of('.') + 1, std::string::npos);

                            DrawText(weightString.c_str(), (int)(infoPos.x - coll.aabb.width * 1.2f), (int)(infoPos.y - coll.aabb.height * 1.2f), 10, PURPLE);
                            break;
                        }
                    }
                }
            }
        }
    }
}
