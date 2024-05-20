#include "npc_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "item_data.hpp"
#include "lua/lualoader.hpp"
#include "raylib.h"
#include "raymath.h"
#include "tile_data.hpp"
#include "tile_manager.hpp"
#include "input_system.hpp"
#include <queue>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>

NPCSystem::NPCSystem(TileManager* tileManager, entt::basic_registry<>* EntityRegistry, entt::entity player)
{
    tManager = tileManager;
    sRegistry = EntityRegistry;
    playerID = player;
}

// spawn NPCs for debugging
void NPCSystem::addNPCs()
{
    int npcSpawnRadius = LuaGetInt("NPC_SPAWN_RADIUS", "scripts/game_settings.lua");
    int geneMax = LuaGetInt("NPC_GENE_MAX_DESIRE", "scripts/game_settings.lua");
    int geneMin = LuaGetInt("NPC_GENE_MIN_DESIRE", "scripts/game_settings.lua");
    float geneIncr = LuaGetFloat("NPC_GENE_INCREMENT", "scripts/game_settings.lua");
    float geneDecr = LuaGetFloat("NPC_GENE_DECREMENT", "scripts/game_settings.lua");

    for (int i = 0; i < LuaGetInt("MAX_NPCS", "scripts/game_settings.lua"); i++) {
        entt::entity entity = sRegistry->create();
        Vector2 pos = {GetRandomValue(-npcSpawnRadius, npcSpawnRadius), GetRandomValue(-npcSpawnRadius, npcSpawnRadius)};
        /* Vector2 pos = {1, 1}; */
        sRegistry->emplace<SpriteC>(entity, AtlasType::SMALL, Rectangle{4, 4, 16, 16});
        sRegistry->emplace<AnimationC>(entity, Rectangle{4, 4, 16, 16}, 4, 4);
        sRegistry->emplace<PositionC>(entity, getGridToScreenPos(pos));
        sRegistry->emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 60, 60, false);
        sRegistry->emplace<CollisionC>(entity, Rectangle{0, 0, 16, 16});

        GenesC geneSet = {
            {geneMax, geneMax, geneMax, geneMax},
            {geneMin, geneMin, geneMin, geneMin},
            {geneIncr, geneIncr, geneIncr, geneIncr},
            {geneDecr, geneDecr, geneDecr, geneDecr},
        };

        sRegistry->emplace<GenesC>(entity, geneSet);
        sRegistry->emplace<NeedsC>(entity, NeedsC{{(float)GetRandomValue(1, 10) / 10.0f, 0.3f, 0.1f, 0.1f, 0.3f}, {geneMax, geneMax, geneMax, geneMax, geneMax}});
        sRegistry->emplace<TimerC>(entity);

        Vector2 randomPath = {GetRandomValue(-20, 20), GetRandomValue(-20, 20)};
        sRegistry->emplace<PathC>(entity, Vector2{-15, -1}, false, false);

        cachePosition(pos, entity);
    }
}

void NPCSystem::update(Scene& scene)
{
    // tell the npc what to do
    updateNPCPaths();
}

bool NPCSystem::entityAtPosition(Vector2 pos)
{
    if (tManager->entityPositionCache.count(pos)) {
        return tManager->entityPositionCache[pos].size() > 0;
    }

    return false;
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
    auto& position = sRegistry->get<PositionC>(id);
    auto& need = sRegistry->get<NeedsC>(id);
    Vector2 centeredPos = {position.pos.x + 8, position.pos.y + 8};
    if (!path.destQueue.empty()) {
        path.isPathing = true;
        path.atTarget = false;

        Vector2 npcGridPos = getGridPosition(centeredPos);

        Vector2 dest = path.destQueue.front();
        Vector2 destAbsolute = getGridToScreenPos(dest);

        Vector2 dir = {(int)destAbsolute.x - (int)position.pos.x, (int)destAbsolute.y - (int)position.pos.y};
        dir = Vector2Normalize(dir);

        if (destAbsolute.x == (int)position.pos.x && destAbsolute.y == (int)position.pos.y) {
            path.destQueue.pop_front();
            /* cachePosition(getGridPosition(centeredPos), id); */
        }
        else {
            /* clearCachePosition(getGridPosition(centeredPos), id); */
            physics.velocity.x = dir.x * physics.speed;
            physics.velocity.y = dir.y * physics.speed;
        }
    }
    else {
        if (need.search) {
            need.search = false;
            path.atTarget = true;
        }
        path.isPathing = false;
        physics.velocity.x = 0;
        physics.velocity.y = 0;
    }
}

BoolVec2Pair NPCSystem::searchItem(entt::entity id, int itemID)
{
    auto& path = sRegistry->get<PathC>(id);
    auto& position = sRegistry->get<PositionC>(id);
    Vector2 initPos = getGridPosition(Vector2{position.pos.x + 8, position.pos.y + 8});

    IndexPair indexPair = tManager->getIndexPair(initPos.x * 16, initPos.y * 16);
    int zLevel = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];
    if (zLevel == 1) {
        return {false, initPos};
    }

    if (path.destQueue.empty()) {
        std::vector<Vector2> nearNeighbors = getSpiralNeighbors(initPos, 10);

        for (Vector2& n : nearNeighbors) {
            IndexPair indexPair = tManager->getIndexPair(n.x * 16, n.y * 16);
            int zLevel = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];

            if (zLevel == 1) {
                continue;
            }

            int item = tManager->chunks[indexPair.chunk].itemID[indexPair.tile];
            if (item == itemID) {
                return {true, n};
            }
        }
    }

    return {false, initPos};
}

bool NPCSystem::nearEntity(entt::entity id)
{

    auto& path = sRegistry->get<PathC>(id);
    auto& position = sRegistry->get<PositionC>(id);
    Vector2 initPos = getGridPosition(Vector2{position.pos.x + 8, position.pos.y + 8});

    IndexPair indexPair = tManager->getIndexPair(initPos.x * 16, initPos.y * 16);
    int zLevel = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];
    if (zLevel == 1) {
        return false;
    }

    std::vector<Vector2> nearNeighbors = getSpiralNeighbors(initPos, 20);

    for (Vector2& n : nearNeighbors) {
        IndexPair indexPair = tManager->getIndexPair(n.x * 16, n.y * 16);
        int zLevel = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];

        if (zLevel == 1) {
            continue;
        }

        // need to check if found entity is not the current entity
        if (entityAtPosition(n) && tManager->entityPositionCache[n][0] != id) {
            return true;
        }
    }

    return false;
}

// astar algorithm to path towards npc's path target, return true boolean if path is found
bool NPCSystem::astar(entt::entity id)
{
    auto& path = sRegistry->get<PathC>(id);
    auto& need = sRegistry->get<NeedsC>(id);

    if (need.search && path.destQueue.empty()) {
        IndexPair indexPair = tManager->getIndexPair(path.target.x * 16, path.target.y * 16);
        int zID = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];
        if (zID == 1) { // if the target is impossible to get to, return
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

    return false; // target was not found
}

// reconstruct path given a node's previous min cost node
void NPCSystem::reconstructPath(PathMap cameFrom, Vector2 current, entt::entity id)
{
    auto& path = sRegistry->get<PathC>(id);

    path.destQueue.push_front(current);
    while (cameFrom.count(current)) {
        path.destQueue.push_front(cameFrom[current]);
        current = cameFrom[current];
    }

    if (!path.destQueue.empty()) {
        path.destQueue.pop_front();
    }
}

// decide path decisions for all npcs
void NPCSystem::updateNPCPaths()
{
    auto view = sRegistry->view<GenesC, NeedsC, PathC>();

    // loop over every npc id
    for (auto id : view) {
        auto& need = view.get<NeedsC>(id);

        moveNPC(id);
        switch (need.currentDesire) {
        case ENERGY: {
            handleLeisure(id);
            break;
        }
        case SATIATION: {
            handleEating(id);
            break;
        }
        case SOCIAL: {
            handleSocial(id);
            break;
        }
        default: {
            break;
        }
        }
        // continuously update aroundFriends status
        if (nearEntity(id)) {
            need.aroundFriends = true;
            need.search = false;

            auto& physics = sRegistry->get<PhysicsC>(id);
            /* physics.velocity.x = 0; */
            /* physics.velocity.y = 0; */
        }
        else {
            need.aroundFriends = false;
        }

        showDebugInfo(id);
    }
}

void NPCSystem::handleEating(entt::entity id)
{
    auto& need = sRegistry->get<NeedsC>(id);
    auto& path = sRegistry->get<PathC>(id);
    auto& position = sRegistry->get<PositionC>(id);

    bool atItem = npcAtItem(id, MUSHROOM_PURPLE);
    if (atItem) {
        need.search = false;
        need.eating = true;
    }
    else {
        need.eating = false;
        if (!path.isPathing) { // if the npc isn't already moving
            BoolVec2Pair searchResponse = searchItem(id, MUSHROOM_PURPLE);
            if (searchResponse.confirm) {
                need.search = true;
                need.leisure = false;
                path.target = searchResponse.pos;
                astar(id);
            }
            else if (!need.search) {
                need.search = true;
                Vector2 gridPos = getGridPosition(position.pos);
                path.target = {GetRandomValue((int)gridPos.x - 10, (int)gridPos.x + 10), GetRandomValue((int)gridPos.y - 10, (int)gridPos.y + 10)};
                astar(id);
            }
        }
    }

    /* if (atItem && need.eating) { */
    /*     Vector2 gridPos = getGridPosition(Vector2{position.pos.x + 8, position.pos.y + 8}); */
    /*     IndexPair indexPair = tManager->getIndexPair(gridPos.x * 16, gridPos.y * 16); */
    /*     tManager->chunks[indexPair.chunk].deleteAtTile(gridPos.x, gridPos.y); */
    /*     need.eating = false; */
    /* } */
}

void NPCSystem::handleLeisure(entt::entity id)
{
    auto& need = sRegistry->get<NeedsC>(id);
    auto& physics = sRegistry->get<PhysicsC>(id);

    physics.velocity.x = 0;
    physics.velocity.y = 0;

    need.search = false;
    need.leisure = true;
}

void NPCSystem::handleSocial(entt::entity id)
{
    auto& need = sRegistry->get<NeedsC>(id);
    auto& path = sRegistry->get<PathC>(id);
    auto& position = sRegistry->get<PositionC>(id);
    auto& physics = sRegistry->get<PhysicsC>(id);

    if (!need.search) {
        need.search = true;
        Vector2 gridPos = getGridPosition(position.pos);
        path.target = {GetRandomValue((int)gridPos.x - 10, (int)gridPos.x + 10), GetRandomValue((int)gridPos.y - 10, (int)gridPos.y + 10)};
        astar(id);
    }
    else if (need.search && need.aroundFriends) {
        physics.velocity.x = 0;
        physics.velocity.y = 0;
        path.destQueue.clear();
    }
}

// checks to see if npc is at an item
bool NPCSystem::npcAtItem(entt::entity id, int itemID)
{
    auto& position = sRegistry->get<PositionC>(id);
    Vector2 curPos = getGridPosition(Vector2{position.pos.x + 8, position.pos.y + 8});

    IndexPair indexPair = tManager->getIndexPair(curPos.x * 16, curPos.y * 16);
    int item = tManager->chunks[indexPair.chunk].itemID[indexPair.tile];

    return item == itemID;
}

void NPCSystem::showDebugInfo(entt::entity id)
{
    PathC& path = sRegistry->get<PathC>(id);
    NeedsC& need = sRegistry->get<NeedsC>(id);
    PositionC& position = sRegistry->get<PositionC>(id);

    // get potential new path
    if (NPC_DEBUG_INFO) {
        Color desireColor;

        if (need.aroundFriends) {
            desireColor = YELLOW;
            DrawTextEx(GetFontDefault(), "SOCIAL", {position.pos.x, position.pos.y - 10}, 3.0f, 0.4f, YELLOW);
        }

        if (need.search) { // drawing a line for debugging visual
            desireColor = RED;
            Vector2 centeredTarget = getGridToScreenPos(path.target);
            Vector2 centeredPos = {position.pos.x + 8, position.pos.y + 8};
            centeredTarget = {centeredTarget.x + 8, centeredTarget.y + 8};

            DrawLineEx(centeredPos, centeredTarget, 1.0f, WHITE);      // drawing path line for debugging
            DrawCircle(centeredTarget.x, centeredTarget.y, 1.5f, RED); // drawing circle at path target
            DrawTextEx(GetFontDefault(), "SEARCH", {position.pos.x, position.pos.y - 5}, 4.0f, 0.1f, desireColor);
        }

        if (need.eating) {
            desireColor = GREEN;
            DrawTextEx(GetFontDefault(), "EATING", {position.pos.x, position.pos.y - 15}, 4.0f, 0.1f, desireColor);
        }

        if (need.leisure) {
            desireColor = PURPLE;
            DrawTextEx(GetFontDefault(), "LEISURE", {position.pos.x, position.pos.y - 1}, 4.0f, 0.1f, desireColor);
        }
    }
}
void NPCSystem::clearCacheBefore()
{
    auto view = sRegistry->view<GenesC, NeedsC, PathC, PhysicsC, PositionC>();

    for (auto& id : view) {
        auto& physics = view.get<PhysicsC>(id);
        auto& position = view.get<PositionC>(id);

        // if npc is moving we need to update cache so we clear the cache of the current pos right now
        if (physics.moving()) {
            auto& position = view.get<PositionC>(id);
            Vector2 gridPos = getGridPosition(Vector2{position.pos.x + 8, position.pos.y + 8});
            clearCachePosition(gridPos, id);
            entities_marked_for_cache.push(id);
        }
    }
}

void NPCSystem::updateCacheAfter()
{
    while (!entities_marked_for_cache.empty()) {
        entt::entity id = entities_marked_for_cache.front();
        auto& position = sRegistry->get<PositionC>(id);
        Vector2 gridPos = getGridPosition({position.pos.x + 8, position.pos.y + 8});

        cachePosition(gridPos, id);
        entities_marked_for_cache.pop();
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
                        PathC& path = sRegistry->get<PathC>(id);
                        CollisionC& coll = sRegistry->get<CollisionC>(id);

                        Rectangle absoluteAABB = {coll.aabb.x + position.pos.x, coll.aabb.y + position.pos.y, coll.aabb.width, coll.aabb.height};

                        if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), absoluteAABB)) {
                            NeedsC& needs = sRegistry->get<NeedsC>(id);
                            Vector2 infoPos = GetScreenToWorld2D(GetMousePosition(), camera);

                            std::string satiationInfo = std::to_string(needs.desires[needType::SATIATION]);
                            satiationInfo.erase(satiationInfo.find_last_not_of('0') + 1, std::string::npos);
                            satiationInfo.erase(satiationInfo.find_last_not_of('.') + 1, std::string::npos);
                            satiationInfo = "satiation: " + satiationInfo;

                            std::string socialInfo = std::to_string(needs.desires[needType::SOCIAL]);
                            socialInfo.erase(socialInfo.find_last_not_of('0') + 1, std::string::npos);
                            socialInfo.erase(socialInfo.find_last_not_of('.') + 1, std::string::npos);
                            socialInfo = "social: " + socialInfo;

                            std::string energyInfo = std::to_string(needs.desires[ENERGY]);
                            energyInfo.erase(energyInfo.find_last_not_of('0') + 1, std::string::npos);
                            energyInfo.erase(energyInfo.find_last_not_of('.') + 1, std::string::npos);
                            energyInfo = "energy: " + energyInfo;

                            std::string idInfo = "id: " + std::to_string((unsigned int)id);
                            std::string targetInfo = "ptarget: " + std::to_string(path.targetID);

                            DrawText(satiationInfo.c_str(), (int)(infoPos.x - coll.aabb.width * 1.2f), (int)(infoPos.y - coll.aabb.height * 1.2f), 10, RED);
                            DrawText(socialInfo.c_str(), (int)(infoPos.x - coll.aabb.width * 1.2f), (int)(infoPos.y - coll.aabb.height * 1.8f), 10, RED);
                            DrawText(energyInfo.c_str(), (int)(infoPos.x - coll.aabb.width * 1.2f), (int)(infoPos.y - coll.aabb.height * 2.4), 10, RED);
                            DrawText(idInfo.c_str(), (int)(infoPos.x - coll.aabb.width * 1.2f), (int)(infoPos.y - coll.aabb.height * 3.0f), 10, PURPLE);
                            DrawText(targetInfo.c_str(), (int)(infoPos.x - coll.aabb.width * 1.2f), (int)(infoPos.y - coll.aabb.height * 3.4f), 10, PURPLE);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
