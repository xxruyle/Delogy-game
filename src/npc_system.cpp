#include "npc_system.hpp"
#include "cache_manager.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "input_system.hpp"
#include "item_data.hpp"
#include "lua/lualoader.hpp"
#include "raylib.h"
#include "raymath.h"
#include "tile_manager.hpp"

NPCSystem::NPCSystem(TileManager* tileManager, entt::basic_registry<>* EntityRegistry, entt::entity player)
{
	tManager = tileManager;
	sRegistry = EntityRegistry;
	playerID = player;
	addNPCs();
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
		sRegistry->emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 45, false);
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

		sRegistry->emplace<PathC>(entity);

		CacheManager::cacheEntity(pos, entity);
	}
}

void NPCSystem::update(Scene& scene)
{
	// tell the npc what to do based on its needs
	handleActions();
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

		Vector2 npcGridPos = getGridPosition(centeredPos);

		Vector2 dest = path.destQueue.front();
		Vector2 destAbsolute = getGridToScreenPos(dest);

		Vector2 dir = {(int)destAbsolute.x - (int)position.pos.x, (int)destAbsolute.y - (int)position.pos.y};
		dir = Vector2Normalize(dir);

		if (destAbsolute.x == (int)position.pos.x && destAbsolute.y == (int)position.pos.y) {
			path.destQueue.pop_front();
		}
		else {
			physics.velocity.x = dir.x * physics.speed;
			physics.velocity.y = dir.y * physics.speed;
		}
	}
	else {
		physics.stop();
	}
}

BoolVec2Pair NPCSystem::searchItem(entt::entity id, int itemID, int radius)
{
	auto& path = sRegistry->get<PathC>(id);
	auto& position = sRegistry->get<PositionC>(id);
	auto& physics = sRegistry->get<PhysicsC>(id);
	Vector2 initPos = getGridPosition(Vector2{position.pos.x + 8, position.pos.y + 8});

	IndexPair indexPair = tManager->getIndexPair(initPos.x * 16, initPos.y * 16);
	int zLevel = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];
	if (zLevel == 1) {
		return {false, initPos};
	}

	/* if (path.destQueue.empty()) { */
	std::vector<Vector2> nearNeighbors = getSpiralNeighbors(initPos, radius);

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
	/* } */

	return {false, initPos};
}

bool NPCSystem::nearEntity(entt::entity id, int radius)
{

	auto& path = sRegistry->get<PathC>(id);
	auto& position = sRegistry->get<PositionC>(id);
	Vector2 initPos = getGridPosition(Vector2{position.pos.x + 8, position.pos.y + 8});

	IndexPair indexPair = tManager->getIndexPair(initPos.x * 16, initPos.y * 16);
	int zLevel = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];
	if (zLevel == 1) {
		return false;
	}

	std::vector<Vector2> nearNeighbors = getSpiralNeighbors(initPos, radius);

	for (Vector2& n : nearNeighbors) {
		IndexPair indexPair = tManager->getIndexPair(n.x * 16, n.y * 16);
		int zLevel = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];

		if (zLevel == 1) {
			continue;
		}

		// need to check if found entity is not the current entity
		if (CacheManager::entityAtPosition(n) && ((CacheManager::entityCache[n][0] != id) && CacheManager::entityCache[n].size() == 1)) {
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

	if (path.destQueue.empty() && path.targetAvailable) {
		IndexPair indexPair = tManager->getIndexPair(path.target.x * 16, path.target.y * 16);
		int zID = tManager->chunks[indexPair.chunk].tileZ[indexPair.tile];
		if (zID == 1) { // if the target is impossible to get to, return
			path.targetAvailable = false;
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
				path.targetAvailable = true;
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

	path.targetAvailable = false;
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

// given a desire, determine the action
void NPCSystem::handleActions()
{
	auto view = sRegistry->view<GenesC, NeedsC, PathC>();

	for (auto id : view) {
		auto& need = view.get<NeedsC>(id);

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

		moveNPC(id);
		checkSearching(id);
		showDebugInfo(id);
	}
}

// gives the npc a random path probabilistically dependent on previous path
void NPCSystem::giveRandomPath(entt::entity id)
{

	auto& path = sRegistry->get<PathC>(id);
	auto& position = sRegistry->get<PositionC>(id);
	Vector2 gridPos = getGridPosition(position.pos);

	// random distance
	float rDistance = GetRandomValue(3, 10);

	// random direction (north, south, east, west)
	int rDir = GetRandomValue(0, 4);

	float rx;
	float ry;

	rx = GetRandomValue((int)gridPos.x - rDistance, (int)gridPos.x + rDistance);
	ry = GetRandomValue((int)gridPos.y - rDistance, (int)gridPos.y + rDistance);

	// give entity the random path target
	path.setTarget(Vector2{rx, ry});
}

void NPCSystem::checkSearching(entt::entity id)
{
	auto& physics = sRegistry->get<PhysicsC>(id);
	auto& path = sRegistry->get<PathC>(id);
	auto& need = sRegistry->get<NeedsC>(id);
	if (physics.moving()) {
		need.currentAction = SEARCHING;

		if (need.currentDesire == SATIATION) {
			BoolVec2Pair searchResponse = searchItem(id, MUSHROOM_PURPLE, 3);

			/* if the target is not already food then we check while searching
			this is so that the npc will check for food in the middle of pathing
			and not have to wait till pathing is done
			*/
			if (searchResponse.confirm && tManager->getItemUnder(Vector2{path.target.x * 16.0f, path.target.y * 16.0f}) != MUSHROOM_PURPLE) {
				path.destQueue.clear();
				physics.stop();
				path.setTarget(searchResponse.pos);
			}
		}
	}
}

void NPCSystem::handleEating(entt::entity id)
{
	auto& need = sRegistry->get<NeedsC>(id);
	auto& path = sRegistry->get<PathC>(id);
	auto& position = sRegistry->get<PositionC>(id);
	auto& physics = sRegistry->get<PhysicsC>(id);

	if (!physics.moving()) {
		/* bool atItem = npcAtItem(id, MUSHROOM_PURPLE); */
		bool atItem = tManager->getItemUnder(Vector2{position.pos.x + 8.0f, position.pos.y + 8.0f}) == MUSHROOM_PURPLE;
		if (atItem) {

			need.currentAction = EATING;
			path.destQueue.clear();

			if (nearEntity(id, 3)) {
				need.currentSubAction = SOCIALIZING;
			}
			else {
				need.currentSubAction = NONE;
			}
		}
		else {
			BoolVec2Pair searchResponse = searchItem(id, MUSHROOM_PURPLE, 10);
			if (searchResponse.confirm) {
				path.setTarget(searchResponse.pos);
			}
			else {
				giveRandomPath(id);
			}
			astar(id);
		}
	}
}

void NPCSystem::handleLeisure(entt::entity id)
{
	auto& need = sRegistry->get<NeedsC>(id);
	auto& physics = sRegistry->get<PhysicsC>(id);
	auto& path = sRegistry->get<PathC>(id);
	physics.stop();
	path.destQueue.clear();

	need.currentAction = RESTING;
}

void NPCSystem::handleSocial(entt::entity id)
{
	auto& need = sRegistry->get<NeedsC>(id);
	auto& path = sRegistry->get<PathC>(id);
	auto& position = sRegistry->get<PositionC>(id);
	auto& physics = sRegistry->get<PhysicsC>(id);
	if (nearEntity(id, 10)) {
		physics.stop();
		path.destQueue.clear();
		need.currentAction = SOCIALIZING;
		path.targetAvailable = false;
	}
	else if (!physics.moving()) {
		// random walk
		giveRandomPath(id);
		astar(id);
	}
}

void NPCSystem::showDebugInfo(entt::entity id)
{
	PathC& path = sRegistry->get<PathC>(id);
	NeedsC& need = sRegistry->get<NeedsC>(id);
	PositionC& position = sRegistry->get<PositionC>(id);
	PhysicsC& physics = sRegistry->get<PhysicsC>(id);

	// get potential new path
	if (NPC_DEBUG_INFO) {
		Color desireColor;
		switch (need.currentAction) {
		case RESTING: {
			desireColor = PURPLE;
			DrawTextEx(GetFontDefault(), "RESTING", {position.pos.x, position.pos.y - 10}, 4.0f, 0.1f, desireColor);
			break;
		}
		case EATING: {
			desireColor = GREEN;
			DrawTextEx(GetFontDefault(), "EATING", {position.pos.x, position.pos.y - 10}, 4.0f, 0.1f, desireColor);
			break;
		}
		case SOCIALIZING: {
			desireColor = YELLOW;
			DrawTextEx(GetFontDefault(), "SOCIALIZING", {position.pos.x, position.pos.y - 10}, 3.0f, 0.4f, desireColor);
			break;
		}
		case SEARCHING: {
			desireColor = RED;
			DrawTextEx(GetFontDefault(), "SEARCHING", {position.pos.x, position.pos.y - 10}, 3.0f, 0.4f, desireColor);
			break;
		}
		default: {
			break;
		}
		}

		switch (need.currentDesire) {
		case ENERGY: {
			desireColor = PURPLE;
			DrawTextEx(GetFontDefault(), "ENERGY", {position.pos.x, position.pos.y - 5}, 4.0f, 0.1f, desireColor);
			break;
		}
		case SATIATION: {
			desireColor = GREEN;
			DrawTextEx(GetFontDefault(), "SATIATION", {position.pos.x, position.pos.y - 5}, 4.0f, 0.1f, desireColor);
			break;
		}
		case SOCIAL: {
			desireColor = YELLOW;
			DrawTextEx(GetFontDefault(), "SOCIAL", {position.pos.x, position.pos.y - 5}, 4.0f, 0.4f, desireColor);
			break;
		}
		default: {
			break;
		}
		}

		// DrawTextEx(GetFontDefault(), getVector2String(physics.velocity).c_str(), {position.pos.x, position.pos.y - 20}, 4.0f, 0.4f, RAYWHITE);

		if (path.targetAvailable) {
			Vector2 centeredPos = {position.pos.x + 8.0f, position.pos.y + 8.0f};
			Vector2 centeredTarget = getGridToScreenPos(path.target);
			centeredTarget = {centeredTarget.x + 8, centeredTarget.y + 8};

			DrawLineEx(centeredPos, centeredTarget, 1.0f, WHITE);	   // drawing path line for debugging
			DrawCircle(centeredTarget.x, centeredTarget.y, 1.5f, RED); // drawing circle at path target
		}
	}
}

// clear npcs from the cache manager if they are moving and add them to the npc cache queue
void NPCSystem::clearCacheBefore()
{
	auto view = sRegistry->view<GenesC, NeedsC, PathC, PhysicsC, PositionC>();

	for (auto& id : view) {
		auto& physics = view.get<PhysicsC>(id);
		auto& position = view.get<PositionC>(id);

		// if npc is moving we need to update cache so we clear the cache of the current pos right now
		if (physics.moving()) {
			Vector2 gridPos = getGridPosition(Vector2{position.pos.x + 8, position.pos.y + 8});

			/*clearCachePosition(gridPos, id);*/
			CacheManager::clearCachePosition(gridPos, id);

			entities_marked_for_cache.push(id);
		}
	}
}

// cache npcs that are marked for cache
void NPCSystem::updateCacheAfter()
{
	while (!entities_marked_for_cache.empty()) {
		entt::entity id = entities_marked_for_cache.front();
		auto& position = sRegistry->get<PositionC>(id);
		Vector2 gridPos = getGridPosition({position.pos.x + 8, position.pos.y + 8});

		/*cachePosition(gridPos, id);*/

		CacheManager::cacheEntity(gridPos, id);
		entities_marked_for_cache.pop();
	}
}

bool NPCSystem::showEntityInfo(Camera2D& camera)
{
	if (InputSystem::getUserKeydown() == SHOW_INFO) {
		Vector2 pos = getMouseGridPosition(camera);

		for (int x = pos.x - 1; x <= pos.x + 1; x++) {
			for (int y = pos.y - 1; y <= pos.y + 1; y++) {
				Vector2 nearPos = {(float)x, (float)y};
				/* std::cout << getVector2String(nearPos) << std::endl; */
				if (CacheManager::entityCache.count(nearPos)) {
					for (entt::entity id : CacheManager::entityCache[nearPos]) {
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
