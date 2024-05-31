#include "lua/lualoader.hpp"
#include "dev_util.hpp"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
#include <cassert>
#include <string>

namespace Slua {
sol::state lua;

void init(std::string fileName)
{
	lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
	try {
		lua.safe_script_file(fileName);
		std::cout << "Lua file: " << fileName << " loaded" << std::endl;
	}
	catch (const sol::error& e) {
		std::cout << std::string(e.what()) << std::endl;
	}
}

} // namespace Slua
