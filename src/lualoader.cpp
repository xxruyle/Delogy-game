#include "lua/lualoader.hpp"
#include "dev_util.hpp"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
#include <cassert>
#include <string>

namespace Slua {
sol::state lua;

void init(std::vector<std::string> fileNames)
{

	lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
	for (std::string& file : fileNames) {
		try {
			lua.safe_script_file(file);
			std::cout << "LUALOADER[SOL]: "
					  << "Loading script from: [" << file << "]" << std::endl;
		}
		catch (const sol::error& e) {
			std::cout << "LUALOADER[SOL]: " << std::string(e.what()) << std::endl;
		}
	}
}

} // namespace Slua
