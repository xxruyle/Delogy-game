#pragma once
#include "sol/sol.hpp"
#include <inttypes.h>
#include <iostream>
#include <raylib.h >
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "luaconf.h"
#include "lualib.h"
}

namespace Slua {
extern sol::state lua;
void init(std::vector<std::string> fileNames);

} // namespace Slua
