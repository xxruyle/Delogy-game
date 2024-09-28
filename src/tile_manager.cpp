#include "tile_manager.hpp"
#include "FastNoiseLite.h"
#include "atlas_data.hpp"
#include "cache_manager.hpp"
#include "keybindings.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "ecs_registry.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "event_manager.hpp"
#include "input_system.hpp"
#include "item_manager.hpp"
#include "lua/lualoader.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cstddef>

int getIndex(int x, int y) { return (CHUNK_SIZE * y) + x; };
Vector2 getRandomDirection()
{
	enum cardinalDirection { NORTH, EAST, SOUTH, WEST };

	int randomDirection = GetRandomValue(0, 3);
	switch (randomDirection) {
	case NORTH:
		return Vector2{0, -1};
		break;
	case EAST:
		return Vector2{1, 0};
		break;
	case SOUTH:
		return Vector2{0, 1};
		break;
	case WEST:
		return Vector2{-1, 0};
		break;
	}
}

/* ---------------- TileChunk Methods  ----------------*/
void TileChunk::wallGeneration()
{
	// fill chunk with wall tiles
	for (int y = 0; y < CHUNK_SIZE; y++) {
		for (int x = 0; x < CHUNK_SIZE; x++) {
			int index = getIndex(x, y);
			tileID[index] = AtlasData::tileids[WALL_FRONT].id;
			tileZ[index] = 1;
		}
	}
}

void TileChunk::draw(Atlas& atlas)
{
	for (int y = 0; y < CHUNK_SIZE; y++) {
		for (int x = 0; x < CHUNK_SIZE; x++) {
			drawTile(atlas, x, y);
			drawItem(atlas, x, y);
		}
	}
}

void TileChunk::drawTile(Atlas& atlas, int x, int y)
{
	int index = getIndex(x, y);
	int id = tileID[index];

	Tile curTile = AtlasData::tileids[id];
	float xAtlasPos = curTile.x;
	float yAtlasPos = curTile.y;

	Rectangle tileAtlasPos = Rectangle{xAtlasPos, yAtlasPos, 16.0f, 16.0f};
	Vector2 loc;
	loc.x = (float)(((float)x + ((float)srcCoordinate.x * (float)CHUNK_SIZE)) * 16.0f);
	loc.y = (float)(((float)y + ((float)srcCoordinate.y * (float)CHUNK_SIZE)) * 16.0f);
	Rectangle tileDest = {loc.x, loc.y, 16.0f, 16.0f};
	DrawTexturePro(atlas.texture, tileAtlasPos, tileDest, {0, 0}, 0.0f, WHITE);
}

void TileChunk::drawItem(Atlas& atlas, int x, int y)
{
	int index = getIndex(x, y);
	int itemid = itemID[index];
	if (itemid != 0) // if an item exists here
	{
		Item curItem = AtlasData::itemids[itemid];
		float xAtlasPos = curItem.x;
		float yAtlasPos = curItem.y;

		Rectangle tileAtlasPos = Rectangle{xAtlasPos, yAtlasPos, 16.0f, 16.0f};
		Vector2 loc;
		loc.x = (float)(((float)x + ((float)srcCoordinate.x * (float)CHUNK_SIZE)) * 16.0f);
		loc.y = (float)(((float)y + ((float)srcCoordinate.y * (float)CHUNK_SIZE)) * 16.0f);
		Rectangle tileDest = {loc.x, loc.y, 16.0f, 16.0f};
		DrawTexturePro(atlas.texture, tileAtlasPos, tileDest, {0, 0}, 0.0f, WHITE);

		// just for debugging
		int amount = itemAmount[index];

		DrawText(std::to_string(amount).c_str(), (int)loc.x, (int)loc.y, 2, RAYWHITE);
		/*Vector2 worldPos = relativeChunkPosToGrid(Vector2{x, y}, srcCoordinate);*/
		/*if (id != entt::null) {*/
		/*	ItemC& item = ECS::registry.get<ItemC>(id);*/
		/**/
		/*	DrawText(std::to_string((unsigned int)item.capacity).c_str(), (int)loc.x, (int)loc.y, 2, RAYWHITE);*/
		/*}*/
	}
}

// deletes item at tile
ItemType TileChunk::deleteAtTile(int x, int y)
{
	int index = getIndex(x, y);
	if (itemID[index] != 0) {	  // if an item exists at this spot
		int item = itemID[index]; // get item id copy
		itemAmount[index] -= 1;
		if (itemAmount[index] <= 0) {
			itemID[index] = 0;
		}

		return (ItemType)item;
	}
	else {
		int zLevel = tileZ[index]; // only delete the tile if z level is > 0
		if (zLevel > 0) {
			tileID[index] = AtlasData::tileids[DIRT_FLOOR_MIDDLE].id;
			tileZ[index] = 0;
		}
	}
	return NULL_ITEM;
}

void TileChunk::updateTile(int x, int y)
{
	int index = getIndex(x, y);
	tileID[index] = AtlasData::tileids[CAVE_FLOOR_MIDDLE].id;
	// we don't need to change z level because we are just replacing the highest
	// block
}

void TileChunk::updateItem(int x, int y, int playerItemID)
{
	int index = getIndex(x, y);
	int curItemID = itemID[index];
	Item newItem = AtlasData::itemids[playerItemID];

	if (tileZ[index] == 0) {
		itemID[index] = newItem.id;
		itemAmount[index] += 1;
	}
}

/* ---------------- TileManager Methods  ----------------*/
void TileManager::generateChunks()
{
	for (int y = -worldSize; y < worldSize; y++) {
		for (int x = -worldSize; x < worldSize; x++) {
			Vector2 chunkCoord = {(float)x, (float)y};

			TileChunk chunk(chunkCoord, world_seed);
			chunks.push_back(chunk);
		}
	}

	drunkardGenerateAll();
	generateOres();
	generateVegetation();
}

void TileManager::generateOres()
{
	for (int y = -worldSize * CHUNK_SIZE; y < worldSize * CHUNK_SIZE; y++) {
		for (int x = -worldSize * CHUNK_SIZE; x < worldSize * CHUNK_SIZE; x++) {
			int rand = GetRandomValue(0, 10000);
			if (rand < 2) {
				int randOreSize = GetRandomValue(5, 15);

				Vector2 curTile = {x, y};
				while (randOreSize >= 0) {
					IndexPair index = getGridIndexPair(curTile.x, curTile.y);
					if (chunks[index.chunk].tileID[index.tile] == WALL_FRONT) { // if a wall
						chunks[index.chunk].tileID[index.tile] = CAVE_FLOOR_MIDDLE;
						randOreSize--;
					}
					else if (chunks[index.chunk].tileID[index.tile] == DIRT_FLOOR_MIDDLE) {
						break;
					}

					curTile = Vector2Add(curTile, getRandomDirection());
					if (!isValidCoordinate(curTile.x, curTile.y)) {
						break;
					}
				}
			}
		}
	}
}

void TileManager::generateVegetation()
{

	for (int y = -worldSize * CHUNK_SIZE; y < worldSize * CHUNK_SIZE; y++) {
		for (int x = -worldSize * CHUNK_SIZE; x < worldSize * CHUNK_SIZE; x++) {
			int rand = GetRandomValue(0, 500);
			if (rand < 1) {
				int randSize = GetRandomValue(10, 20);
				Vector2 curTile = {x, y};
				IndexPair index = getGridIndexPair(curTile.x, curTile.y);
				while (randSize >= 0) {
					IndexPair index = getGridIndexPair(curTile.x, curTile.y);
					if (chunks[index.chunk].tileID[index.tile] == DIRT_FLOOR_MIDDLE) { // if a dirt tile
						chunks[index.chunk].itemID[index.tile] = MUSHROOM_PURPLE;
						chunks[index.chunk].itemAmount[index.tile] = 1;
						randSize--;
					}
					else if (chunks[index.chunk].tileID[index.tile] == WALL_FRONT) {
						break;
					}

					curTile = Vector2Add(curTile, getRandomDirection());
					if (!isValidCoordinate(curTile.x, curTile.y)) {
						break;
					}
				}
			}
		}
	}
}

bool TileManager::chunkExists(Vector2 chunkPos) { return (chunkPos.x >= -worldSize && chunkPos.x < worldSize) && (chunkPos.y >= -worldSize && chunkPos.y < worldSize); }

int TileManager::getChunkIndex(int x, int y)
{ // given the coordinate of a chunk
	int stride = (worldSize * 2 - 1);
	x = worldSize + x;
	y = worldSize + y;
	return x + (y * (stride + 1));
}

void TileManager::checkDevInput()
{
	if (IsKeyDown(KEY_UP)) {
		renderDistance++;
	}
	else if (IsKeyDown(KEY_DOWN)) {
		renderDistance--;
	}
}

void TileManager::checkPlayerInteraction(Camera2D& camera, UI& ui, InventoryC& playerInventory, HotBarC& hotBar)
{
	int interactKey = InputSystem::getUserMouseInteraction();
	if (interactKey) {
		Vector2 mousePos = getMouseGridPosition(camera);

		Vector2 chunkPos = getMouseChunkPosition(camera);

		int chunkIndex = getChunkIndex(chunkPos.x, chunkPos.y);
		Vector2 relativeGridPos = getRelativeChunkGridPosition(chunkPos, mousePos);

		if (chunkExists(chunkPos) && ui.mouseOutOfBounds()) { // player can interact if chunk exists and
			// mouse is not over ui
			switch (interactKey) {
			case PLAYER_DESTROY: {
				chunks[chunkIndex].deleteAtTile(relativeGridPos.x, relativeGridPos.y);
				updatedChunks.push_back(chunkIndex);
				break;
			}
			case PLAYER_CREATE: {
				int selectedItem = playerInventory.slots[hotBar.curItem];
				if (selectedItem >= RAIL_NW && selectedItem <= MUSHROOM_PURPLE) {
					chunks[chunkIndex].updateItem(relativeGridPos.x, relativeGridPos.y, selectedItem);
					updatedChunks.push_back(chunkIndex);
				}
				break;
			}
			default:
				break;
			}
		}
	}
}

std::vector<Vector2> TileManager::getNearbyChunks(Vector2 playerPos, int distance)
{
	// getting which chunk the player is in
	Vector2 playerChunkLocation = getChunkPosition(getGridPosition(playerPos));
	std::vector<Vector2> chunkBuffer;

	// getting the chunk positions around the player
	for (int i = playerChunkLocation.y - distance; i < playerChunkLocation.y + distance; i++) {
		for (int j = playerChunkLocation.x - distance; j < playerChunkLocation.x + distance; j++) {
			// getting the chunk world position
			Vector2 chunkPos = chunkToWorldSpace(Vector2{j, i});

			// put the chunk position in the middle of the chunk
			chunkPos.x += (float)CHUNK_SIZE / 2 * 16;
			chunkPos.y += (float)CHUNK_SIZE / 2 * 16;

			if (Vector2Distance(chunkPos, playerPos) < distance * CHUNK_SIZE * 16) {
				Vector2 nearChunk = {j, i};
				chunkBuffer.push_back(nearChunk);
			}
		}
	}
	return chunkBuffer;
}

void TileManager::drawAllChunks(Atlas& atlas, Vector2& playerPos)
{
	std::vector<Vector2> chunkBuffer = getNearbyChunks(playerPos, renderDistance);

	for (std::vector<Vector2>::size_type i = 0; i < chunkBuffer.size(); i++) {
		Vector2 chunkPos = chunkToWorldSpace(chunkBuffer[i]);

		std::vector<Vector2>::size_type index = getChunkIndex(chunkBuffer[i].x, chunkBuffer[i].y);

		if (index < chunks.size() && index >= 0 && chunkExists(chunkBuffer[i])) { // checking out of bounds
			chunks[index].draw(atlas);
		}
	}
}

void TileManager::update(Atlas& atlas, UI& ui, Scene& scene)
{
	PositionC& position = ECS::registry.get<PositionC>(scene.player);
	InventoryC& inventory = ECS::registry.get<InventoryC>(scene.player);
	HotBarC& hotBar = ECS::registry.get<HotBarC>(scene.player);
	drawAllChunks(atlas, position.pos);
	/*checkPlayerInteraction(scene.camera, ui, inventory, hotBar);*/
	checkDevInput();
	handleEvents();
}

int TileManager::getItemUnder(Vector2 pos)
{
	Vector2 centeredPos = {pos.x, pos.y};
	Vector2 gridPos = getGridPosition(centeredPos);
	Vector2 chunkPos = getChunkPosition(gridPos);
	std::vector<Vector2>::size_type index = getChunkIndex(chunkPos.x, chunkPos.y);
	Vector2 relativeChunkGridPos = getRelativeChunkGridPosition(chunkPos, gridPos);
	int relativeChunkIndex = getIndex(relativeChunkGridPos.x, relativeChunkGridPos.y);

	return chunks[index].itemID[relativeChunkIndex];
}

std::vector<Vector2> TileManager::getNeighbors(int x, int y, int radius)
{
	std::vector<Vector2> neighbors;

	for (int row = y - radius; row < y + radius; row++) {
		for (int col = x - radius; col < x + radius; col++) {
			if (isValidCoordinate(col, row)) {
				neighbors.push_back(Vector2{col, row});
			}
		}
	}

	return neighbors;
}

bool TileManager::isValidCoordinate(int x, int y)
{
	int worldBound = CHUNK_SIZE * worldSize;
	return ((x >= -worldBound && y >= -worldBound) && (x < worldBound && y < worldBound));
}

IndexPair TileManager::getIndexPair(int x, int y)
{
	Vector2 centeredPos = {x, y};
	Vector2 gridPos = getGridPosition(centeredPos);
	Vector2 chunkPos = getChunkPosition(gridPos);
	int chunkIndex = getChunkIndex(chunkPos.x, chunkPos.y);
	Vector2 relativeChunkGridPos = getRelativeChunkGridPosition(chunkPos, gridPos);
	int tileIndex = getIndex((int)relativeChunkGridPos.x, (int)relativeChunkGridPos.y);

	return IndexPair{chunkIndex, tileIndex};
}

IndexPair TileManager::getGridIndexPair(int x, int y)
{
	Vector2 chunkPos = getChunkPosition({x, y});
	int chunkIndex = getChunkIndex((int)chunkPos.x, (int)chunkPos.y);
	Vector2 relativeChunkGridPos = getRelativeChunkGridPosition(chunkPos, {x, y});
	int tileIndex = getIndex(relativeChunkGridPos.x, relativeChunkGridPos.y);

	return IndexPair{chunkIndex, tileIndex};
}

void TileManager::drunkardGenerateAll()
{
	int worldLength = CHUNK_SIZE * worldSize;
	int totalTileCount = worldLength * worldLength;
	for (int i = 0; i < worldSize * 4; i++) {
		/* std::cout << i << std::endl; */
		int floorCount = 0;
		int trappedCount = 0;
		Vector2 curTile;
		if (i == 0) {
			Vector2 curTile = Vector2{1, 1};
		}
		else {
			Vector2 curTile = Vector2{GetRandomValue(-worldLength, worldLength), GetRandomValue(-worldLength, worldLength)};
		}

		while (floorCount < 20000) {
			Vector2 chunkPos = getChunkPosition(curTile);
			int chunkIndex = getChunkIndex((int)chunkPos.x, (int)chunkPos.y);
			Vector2 relativeChunkGridPos = getRelativeChunkGridPosition(chunkPos, curTile);
			int tileIndex = getIndex(relativeChunkGridPos.x, relativeChunkGridPos.y);

			if (curTile.x >= -worldLength && curTile.y >= -worldLength && curTile.y < worldLength && curTile.x < worldLength) {
				if (chunks[chunkIndex].tileID[tileIndex] != DIRT_FLOOR_MIDDLE) {
					chunks[chunkIndex].tileID[tileIndex] = AtlasData::tileids[DIRT_FLOOR_MIDDLE].id;
					chunks[chunkIndex].tileZ[tileIndex] = 0;
					floorCount++;
				}
				else {
					trappedCount++;
					if (trappedCount > 5000) {
						break;
					}
				}
			}
			else {
				break;
			}

			curTile = Vector2Add(curTile, getRandomDirection());
		}
	}
}

void TileManager::handleEvents()
{
	if (!EventManager::itemEventQueue.empty()) {
		ItemEvent e = EventManager::itemEventQueue.front();
		switch (e.type) {
		case ITEM_DELETION: {
			Vector2 chunkPos = getChunkPosition(e.itemPos);
			int chunkIndex = getChunkIndex((int)chunkPos.x, (int)chunkPos.y);
			Vector2 relativeChunkPos = getRelativeChunkGridPosition(chunkPos, e.itemPos);
			ItemType itemID = chunks[chunkIndex].deleteAtTile(relativeChunkPos.x, relativeChunkPos.y);
			updatedChunks.push_back(chunkIndex);
			EventManager::itemEventQueue.pop_front();

			if (itemID != NULL_ITEM) {
				// sending a item_get event to the entity destroying
				ItemEvent newE = {ITEM_GET, itemID, e.itemPos, e.relatedEntity};
				EventManager::itemEventQueue.push_back(newE);
			}
			break;
		}
		case ITEM_CREATION: {
			Vector2 chunkPos = getChunkPosition(e.itemPos);
			int chunkIndex = getChunkIndex((int)chunkPos.x, (int)chunkPos.y);
			Vector2 relativeChunkPos = getRelativeChunkGridPosition(chunkPos, e.itemPos);
			chunks[chunkIndex].updateItem(relativeChunkPos.x, relativeChunkPos.y, e.itemID);
			updatedChunks.push_back(chunkIndex);
			EventManager::itemEventQueue.pop_front();
			break;
		}
		default:
			break;
		}
	}
}
