#include "npc_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "raymath.h"
#include "tile_data.hpp"
#include "tile_manager.hpp"
#include "input_system.hpp"
#include <queue>
#include <regex>
#include <string>
#include <unordered_map>

NPCSystem::NPCSystem(TileManager* tileManager, entt::basic_registry<>* EntityRegistry, entt::entity player)
{
    tManager = tileManager;
    sRegistry = EntityRegistry;
    playerID = player;
}

// spawn NPCs for debugging
void NPCSystem::addNPCs()
{
    for (int i = 0; i < 2; i++) {
        entt::entity entity = sRegistry->create();
        Vector2 pos = {GetRandomValue(-10, 10), GetRandomValue(-10, 10)};
        /* Vector2 pos = {1, 1}; */
        sRegistry->emplace<SpriteC>(entity, AtlasType::SMALL, Rectangle{4, 4, 16, 16});
        sRegistry->emplace<AnimationC>(entity, Rectangle{4, 4, 16, 16}, 4, 4);
        sRegistry->emplace<PositionC>(entity, getGridToScreenPos(pos));
        sRegistry->emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 30, 30, false);
        sRegistry->emplace<CollisionC>(entity, Rectangle{0, 0, 16, 16});
        sRegistry->emplace<NeedsC>(entity, NeedsC{{(float)GetRandomValue(1, 10) / 10.0f, 0.3f, 0.1f, 0.1f, 0.3f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}});
        sRegistry->emplace<TimerC>(entity);

        Vector2 randomPath = {GetRandomValue(-20, 20), GetRandomValue(-20, 20)};
        sRegistry->emplace<PathC>(entity, Vector2{-15, -1}, false, true);

        Vector2 entityPos = getGridPosition(pos);
        cachePosition(entityPos, entity);
    }
}
void NPCSystem::update(Scene& scene)
{
    updateNPCPaths();
    updateNeeds();
}

void NPCSystem::updateDesires(NeedsC& need)
{

    for (int needType = 0; needType < 5; needType++) {
        float newNeed = need.desires[needType];
        newNeed += need.weights[needType];

        if (newNeed >= 100.0f) {
            newNeed = 100.0f;
        }

        need.desires[needType] = newNeed;
    }
}

void NPCSystem::updateNeeds()
{
    auto view = sRegistry->view<NeedsC, TimerC>();

    for (auto id : view) {
        auto& needs = view.get<NeedsC>(id);
        auto& timer = view.get<TimerC>(id);

        if (GetTime() - timer.lastTime >= 2.0f) {
            updateDesires(needs);
            timer.lastTime = GetTime();
        }
    }
}

// cache entity location given a position
void NPCSystem::cachePosition(Vector2 pos, entt::entity id) { tManager->entityPositionCache[pos].push_back(id); }

// delete the entity in the cache void
void NPCSystem::clearCachePosition(Vector2 pos, entt::entity id)
{
    // loop over every  entity at position
    for (int i = 0; i < tManager->entityPositionCache[pos].size(); i++) {
        entt::entity entID = tManager->entityPositionCache[pos][i];

        // if the entity id is what we are looking for erase it
        if (entID == id) {
            std::vector<entt::entity>::iterator it = tManager->entityPositionCache[pos].begin() + i;
            tManager->entityPositionCache[pos].erase(it);
        }
    }

    // deleting the position key if no entity exists in it
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

// set npc velocity if there are new paths in its path queue
void NPCSystem::moveNPC(entt::entity id)
{
    auto& path = sRegistry->get<PathC>(id);
    auto& physics = sRegistry->get<PhysicsC>(id);
    if (!path.destQueue.empty()) {
        path.atTarget = false;
        auto& position = sRegistry->get<PositionC>(id);

        Vector2 centeredPos = {position.pos.x + 8, position.pos.y + 8};
        Vector2 npcGridPos = getGridPosition(centeredPos);

        Vector2 dest = path.destQueue.front();
        Vector2 destAbsolute = getGridToScreenPos(dest);

        Vector2 dir = {(int)destAbsolute.x - (int)position.pos.x, (int)destAbsolute.y - (int)position.pos.y};
        dir = Vector2Normalize(dir);

        if (destAbsolute.x == (int)position.pos.x && destAbsolute.y == (int)position.pos.y) {
            path.destQueue.pop_front();
        }
        else {
            DrawTextEx(GetFontDefault(), getVector2String(npcGridPos).c_str(), {position.pos.x, position.pos.y - 5}, 5.0f, 0.1f, RED);
            physics.velocity.x = dir.x * physics.speed;
            physics.velocity.y = dir.y * physics.speed;
        }

        // drawing a line for debugging visual
        Vector2 centeredTarget = getGridToScreenPos(path.target);
        centeredTarget = {centeredTarget.x + 8, centeredTarget.y + 8};
        DrawLineEx(centeredPos, centeredTarget, 1.0f, RAYWHITE);   // drawing path line for debugging
        DrawCircle(centeredTarget.x, centeredTarget.y, 1.5f, RED); // drawing circle at path target

        // updating entity cache
        clearCachePosition(getGridPosition(position.pos), id);
        cachePosition(getGridPosition(position.pos), id);
    }
    else {
        if (path.atTarget == false) {
            auto& needs = sRegistry->get<NeedsC>(id);
            needs.desires[0] = 0;
        }
        path.atTarget = true;
        physics.velocity.x = 0;
        physics.velocity.y = 0;
    }
}

// astar algorithm to path towards npc's path target, return true boolean if path is found
bool NPCSystem::astar(entt::entity id)
{
    auto& path = sRegistry->get<PathC>(id);

    if (path.isPathing) { // check to see if npc is pathing
        IndexPair indexPair = tManager->getIndexPair(path.target.x * 16, path.target.y * 16);
        int zID = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];
        if (zID == 1) { // if the target is impossible to get to, return
            path.isPathing = false;
            return false;
        }

        // initializing astar storages
        PathMap cameFrom;
        PathVisited visited;
        PathQueue fringe;

        auto& position = sRegistry->get<PositionC>(id);
        Vector2 gridPos = getGridPosition(Vector2{position.pos.x + 8, position.pos.y + 8});
        PathNode initialNode = {gridPos, 0};
        fringe.push(initialNode); // initialize first node
        visited[initialNode.pos] = initialNode;

        // astar loop
        while (!fringe.empty()) {
            PathNode curNode = fringe.top();

            // if npc meets path target
            if (curNode.pos.x == path.target.x && curNode.pos.y == path.target.y) {
                reconstructPath(cameFrom, curNode.pos, id);
                path.isPathing = false;
                return true;
            }

            std::vector<Vector2> neighbors = getNearNeighbors(curNode.pos); // get nearby nodes

            fringe.pop();
            for (Vector2& n : neighbors) {
                IndexPair indexPair = tManager->getIndexPair(n.x * 16, n.y * 16);
                int zID = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];

                if (zID != 1) {
                    PathNode neighborNode = {n, curNode.cost + 1 + Vector2Manhattan(n, path.target)};
                    if (visited.count(neighborNode.pos)) {
                        // update if new found node is cheaper
                        if (visited[neighborNode.pos].cost > neighborNode.cost) {
                            visited[neighborNode.pos] = neighborNode;
                            cameFrom[neighborNode.pos] = curNode.pos;
                            fringe.push(neighborNode);
                        }
                    }
                    else {
                        visited[neighborNode.pos] = neighborNode;
                        cameFrom[neighborNode.pos] = curNode.pos;
                        fringe.push(neighborNode);
                    }
                }
            }
        }
    }

    path.isPathing = false;
    return false; // target was not found
}

// reconstruct path given a node's previous min cost node
void NPCSystem::reconstructPath(PathMap cameFrom, Vector2 current, entt::entity id)
{
    auto& path = sRegistry->get<PathC>(id);

    path.destQueue.push_front(current);
    while (cameFrom.count(current)) {
        /* std::cout << getVector2String(current) << std::endl; */
        path.destQueue.push_front(cameFrom[current]);
        current = cameFrom[current];
    }
}

bool NPCSystem::isReadyToPath(PathC& path) { return path.atTarget && !path.isPathing; }

// decide path decisions for all npcs
void NPCSystem::updateNPCPaths()
{
    auto view = sRegistry->view<PositionC, NeedsC, PhysicsC>();

    for (auto id : view) {
        auto& needs = view.get<NeedsC>(id);
        auto& position = view.get<PositionC>(id);

        auto& physics = view.get<PhysicsC>(id);

        auto& path = sRegistry->get<PathC>(id);
        auto& need = sRegistry->get<NeedsC>(id);

        auto& playerCollision = sRegistry->get<CollisionC>(playerID);
        auto& playerPosition = sRegistry->get<PositionC>(playerID);

        // get potential new path
        /* && need.needs[0] == 1.0f */
        if (isReadyToPath(path) && needs.desires[0] >= 1.0f) {
            Vector2 gridPos = getGridPosition(position.pos);
            int randX = GetRandomValue((int)gridPos.x - 10, (int)gridPos.x + 10);
            int randY = GetRandomValue((int)gridPos.x - 10, (int)gridPos.x + 10);
            path.target = {(float)randX, (float)randY};
            path.isPathing = true;
            astar(id);
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

                            std::string weightString = std::to_string(needs.desires[0]);
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
