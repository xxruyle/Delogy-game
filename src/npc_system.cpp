#include "npc_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include "input_system.hpp"
#include <string>

NPCSystem::NPCSystem(TileManager* tileManager) { tManager = tileManager; }

void NPCSystem::addNPCs(entt::basic_registry<>& registry)
{
    for (int i = 0; i < 50; i++) {
        entt::entity entity = registry.create();
        Vector2 pos = {GetRandomValue(-1000, 1000), GetRandomValue(-1000, 1000)};
        registry.emplace<SpriteC>(entity, AtlasType::SMALL, Rectangle{88, 4, 16, 16});
        registry.emplace<PositionC>(entity, pos);
        registry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 5, 5, false);
        registry.emplace<CollisionC>(entity, Rectangle{0, 0, 16, 16});
        registry.emplace<NeedsC>(entity, NeedsC{{(float)GetRandomValue(1, 10) / 10.0f, 0.3f, 0.1f, 0.1f, 0.3f}});

        Vector2 entityPos = getGridPosition(pos);
        cachePosition(entityPos, entity);
    }
}

void NPCSystem::update(Scene& scene) { showEntityInfo(scene.camera, scene.EntityRegistry); }

void NPCSystem::cachePosition(Vector2 pos, entt::entity id) { tManager->entityPositionCache[pos].push_back(id); }

void NPCSystem::clearCachePosition(Vector2 pos, entt::entity id)
{
    for (int i = 0; i < tManager->entityPositionCache[pos].size(); i++) {
        entt::entity entID = tManager->entityPositionCache[pos][i];
        if (entID == id) {
            std::vector<entt::entity>::iterator it = tManager->entityPositionCache[pos].begin() + i;
            tManager->entityPositionCache[pos].erase(it);
        }
    }

    if (!tManager->entityPositionCache.count(pos)) {
        for (auto it = tManager->entityPositionCache.begin(); it != tManager->entityPositionCache.end();) {
            if (it->first.x == pos.x && it->first.y == pos.y) {
                it = tManager->entityPositionCache.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void NPCSystem::moveNPCs(entt::basic_registry<>& registry)
{

    auto view = registry.view<PositionC, NeedsC, PhysicsC>();

    for (auto entity : view) {
        auto& needs = view.get<NeedsC>(entity);
        auto& position = view.get<PositionC>(entity);
        if (GetRandomValue(0, 1000) > 999) {
            Vector2 entityPos = getGridPosition(position.pos);
            clearCachePosition(entityPos, entity);
            position.pos.x += GetRandomValue(1, 4);
            entityPos = getGridPosition(position.pos);
            cachePosition(entityPos, entity);
        }

        auto& physics = view.get<PhysicsC>(entity);
    }
}

bool NPCSystem::showEntityInfo(Camera2D& camera, entt::basic_registry<>& registry)
{
    if (InputSystem::getUserKeydown() == SHOW_INFO) {
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
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
