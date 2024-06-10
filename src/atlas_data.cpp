#include "atlas_data.hpp"

namespace AtlasData {
extern Tile tileids[NUM_TILES] = {};
extern Item itemids[NUM_ITEMS] = {};
void init()
{
	sol::table tiles = Slua::lua["TILES"];
	// tile atlas data
	for (auto& pair : tiles) {
		sol::object key = pair.first;
		sol::table inner_table = pair.second.as<sol::table>();

		std::string tile_name = key.as<std::string>();
		int id = inner_table["id"];
		int x = inner_table["x"];
		int y = inner_table["y"];
		Tile t = {id, x, y};
		tileids[id] = t;
	}

	// item atlas data
	sol::table items = Slua::lua["ITEMS"];
	// Loop over the outer table
	for (auto& pair : items) {
		sol::object key = pair.first;
		sol::table inner_table = pair.second.as<sol::table>();

		int id = inner_table["id"];
		int x = inner_table["x"];
		int y = inner_table["y"];
		Item t = {id, x, y};
		itemids[id] = t;
	}
}
} // namespace AtlasData
