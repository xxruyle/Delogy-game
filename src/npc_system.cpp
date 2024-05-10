#include "npc_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "raymath.h"
#include "tile_manager.hpp"
#include "input_system.hpp"
#include <string>

NPCSystem::NPCSystem(TileManager* tileManager, entt::basic_registry<>* EntityRegistry)
{
    tManager = tileManager;
    sRegistry = EntityRegistry;
}

void NPCSystem::addNPCs()
{
    for (int i = 0; i < 1; i++) {
        entt::entity entity = sRegistry->create();
        /* Vector2 pos = {GetRandomValue(-1000, 1000), GetRandomValue(-1000, 1000)}; */
        Vector2 pos = {0, 0};
        sRegistry->emplace<SpriteC>(entity, AtlasType::SMALL, Rectangle{88, 4, 16, 16});
        sRegistry->emplace<PositionC>(entity, pos);
        sRegistry->emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 30, 30, false);
        sRegistry->emplace<CollisionC>(entity, Rectangle{0, 0, 16, 16});
        sRegistry->emplace<NeedsC>(entity, NeedsC{{(float)GetRandomValue(1, 10) / 10.0f, 0.3f, 0.1f, 0.1f, 0.3f}});
        sRegistry->emplace<PathC>(entity);

        Vector2 entityPos = getGridPosition(pos);
        cachePosition(entityPos, entity);
    }
}

void NPCSystem::update(Scene& scene) { showEntityInfo(scene.camera); }

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

// move particular npc to a destination
void NPCSystem::moveNPC(entt::entity id)
{
    auto& path = sRegistry->get<PathC>(id);
    if (!path.destQueue.empty()) {
        auto& position = sRegistry->get<PositionC>(id);
        auto& physics = sRegistry->get<PhysicsC>(id);

        Vector2 npcGridPos = getGridPosition(position.pos);

        Vector2 dest = path.destQueue.front();

        Vector2 dir = {dest.x - npcGridPos.x, dest.y - npcGridPos.y};
        dir = Vector2Normalize(dir);

        if (dest.x == npcGridPos.x && dest.y == npcGridPos.y) {
            path.destQueue.pop();
        }
        else {
            DrawText(getVector2String(dest).c_str(), position.pos.x, position.pos.y - 10, 10, RED);
            physics.velocity.x = dir.x * physics.speed;
            physics.velocity.y = dir.y * physics.speed;
        }

        // drawing a line for debugging visual
        Vector2 centeredDest = getGridToScreenPos(dest);
        centeredDest = {centeredDest.x + 8, centeredDest.y + 8};
        DrawLineV(centeredDest, position.pos, PURPLE);
        clearCachePosition(getGridPosition(position.pos), id);
        cachePosition(getGridPosition(position.pos), id);
    }
}

void NPCSystem::moveNPCs()
{

    auto view = sRegistry->view<PositionC, NeedsC, PhysicsC>();

    for (auto id : view) {
        auto& needs = view.get<NeedsC>(id);
        auto& position = view.get<PositionC>(id);

        auto& physics = view.get<PhysicsC>(id);
        auto& path = sRegistry->get<PathC>(id);

        if (path.destQueue.empty()) {
            path.destQueue.push(Vector2{0, 0});
            path.destQueue.push(Vector2{0, -1});
            path.destQueue.push(Vector2{-1, -1});
            path.destQueue.push(Vector2{-1, -2});
            path.destQueue.push(Vector2{-2, -2});
            path.destQueue.push(Vector2{-2, -3});
            path.destQueue.push(Vector2{-3, -3});
            path.destQueue.push(Vector2{-3, -4});
            path.destQueue.push(Vector2{-4, -4});
            path.destQueue.push(Vector2{-4, -5});
        }
        moveNPC(id);
    }
}

bool NPCSystem::showEntityInfo(Camera2D& camera)
{
    if (InputSystem::getUserKeydown() == SHOW_INFO) {
        Vector2 pos = getMouseGridPosition(camera);

        for (int x = pos.x - 1; x < pos.x + 1; x++) {
            for (int y = pos.y - 1; y < pos.y + 1; y++) {
                Vector2 nearPos = {(float)x, (float)y};
                /* std::cout << getVector2String(nearPos) << std::endl; */
                if (tManager->entityPositionCache.count(nearPos)) {
                    for (entt::entity id : tManager->entityPositionCache[nearPos]) {
                        PositionC& position = sRegistry->get<PositionC>(id);
                        CollisionC& coll = sRegistry->get<CollisionC>(id);

                        Rectangle absoluteAABB = {coll.aabb.x + position.pos.x, coll.aabb.y + position.pos.y, coll.aabb.width, coll.aabb.height};

                        if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), absoluteAABB)) {
                            NeedsC& needs = sRegistry->get<NeedsC>(id);
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
