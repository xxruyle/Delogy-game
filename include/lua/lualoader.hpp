#pragma once
#include <inttypes.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "luaconf.h"
#include "lualib.h"
}

void InitLua();
void CloseLua();

std::string LuaGetString(const char *varName, const char *filePath);
int LuaGetInt(const char *varName, const char *filePath);
float LuaGetFloat(const char *varName, const char *filePath);
bool LuaGetBool(const char *varName, const char *filePath);

std::string LuaGetTableString(const char *tableName, const char *tableKey,
                              const char *filePath);

int LuaGetTableInt(const char *tableName, const char *tableKey,
                   const char *filePath);

bool CheckLua(lua_State *L, int response);
