#include "npc_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
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

void NPCSystem::addNPCs()
{ // spawn NPCs for debugging
    for (int i = 0; i < 50; i++) {
        entt::entity entity = sRegistry->create();
        Vector2 pos = {GetRandomValue(-10, 10), GetRandomValue(-10, 10)};
        /* Vector2 pos = {1, 1}; */
        sRegistry->emplace<SpriteC>(entity, AtlasType::SMALL, Rectangle{88, 4, 16, 16});
        sRegistry->emplace<PositionC>(entity, getGridToScreenPos(pos));
        sRegistry->emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 30, 30, false);
        sRegistry->emplace<CollisionC>(entity, Rectangle{0, 0, 16, 16});
        sRegistry->emplace<NeedsC>(entity, NeedsC{{(float)GetRandomValue(1, 10) / 10.0f, 0.3f, 0.1f, 0.1f, 0.3f}});

        Vector2 randomPath = {GetRandomValue(-20, 20), GetRandomValue(-20, 20)};
        sRegistry->emplace<PathC>(entity, Vector2{-15, -1}, false, true);

        Vector2 entityPos = getGridPosition(pos);
        cachePosition(entityPos, entity);
    }
}

void NPCSystem::update(Scene& scene) { moveNPCs(); }

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
            DrawText(getVector2String(npcGridPos).c_str(), position.pos.x, position.pos.y - 10, 0.3f, RED);
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
    else {
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

        PathMap cameFrom;
        std::unordered_map<Vector2, PathNode, Vector2Util, Vector2Util> visited;
        std::priority_queue<PathNode, std::vector<PathNode>, PathNodeComparison> fringe;

        auto& position = sRegistry->get<PositionC>(id);
        Vector2 gridPos = getGridPosition(Vector2{position.pos.x + 8, position.pos.y + 8});
        PathNode initialNode = {gridPos, 0};
        fringe.push(initialNode); // initialize first node
        visited[initialNode.pos] = initialNode;

        while (!fringe.empty()) {
            PathNode curNode = fringe.top();

            if (curNode.pos.x == path.target.x && curNode.pos.y == path.target.y) {
                path.isPathing = false;
                reconstructPath(cameFrom, curNode.pos, id);
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

// decide path decisions for all npcs
void NPCSystem::moveNPCs()
{

    auto view = sRegistry->view<PositionC, NeedsC, PhysicsC>();

    for (auto id : view) {
        auto& needs = view.get<NeedsC>(id);
        auto& position = view.get<PositionC>(id);

        auto& physics = view.get<PhysicsC>(id);

        auto& path = sRegistry->get<PathC>(id);

        auto& playerCollision = sRegistry->get<CollisionC>(playerID);
        auto& playerPosition = sRegistry->get<PositionC>(playerID);

        // get potential new path
        if (path.atTarget) {
            if (path.atTarget && !path.isPathing) {
                Vector2 gridPos = getGridPosition(position.pos);
                int randX = GetRandomValue((int)gridPos.x - 10, (int)gridPos.x + 10);
                int randY = GetRandomValue((int)gridPos.x - 10, (int)gridPos.x + 10);
                path.target = {(float)randX, (float)randY};
                path.isPathing = true;
                astar(id);
            }
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
