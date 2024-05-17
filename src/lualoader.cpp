#include "lua/lualoader.hpp"
#include "dev_util.hpp"
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
#include <string>

lua_State* L = nullptr;

void InitLua()
{
    L = luaL_newstate();
    luaL_openlibs(L);
}

void CloseLua() { lua_close(L); }

std::string LuaGetString(const char* varName, const char* filePath)
{
    std::string strValue;
    if (CheckLua(L, luaL_dofile(L, filePath))) {
        lua_getglobal(L, varName);
        if (lua_isnumber(L, -1)) {
            strValue = lua_tostring(L, -1);
            lua_pop(L, 1);
            return strValue;
        }
    }

    return std::string("null");
}

int LuaGetInt(const char* varName, const char* filePath)
{
    int intValue;
    if (CheckLua(L, luaL_dofile(L, filePath))) {
        lua_getglobal(L, varName);
        if (lua_isnumber(L, -1)) {
            intValue = lua_tointeger(L, -1);
            lua_pop(L, 1);
            return intValue;
        }
    }

    return 0;
}

float LuaGetFloat(const char* varName, const char* filePath)
{
    float floatValue;
    if (CheckLua(L, luaL_dofile(L, filePath))) {
        lua_getglobal(L, varName);
        if (lua_isnumber(L, -1)) {
            floatValue = lua_tonumber(L, -1);
            lua_pop(L, 1);
            return floatValue;
        }
    }

    return 0;
}

bool LuaGetBool(const char* varName, const char* filePath)
{
    bool boolValue;
    if (CheckLua(L, luaL_dofile(L, filePath))) {
        lua_getglobal(L, varName);
        if (lua_isboolean(L, -1)) {
            boolValue = lua_toboolean(L, -1);
            lua_pop(L, 1);
            return boolValue;
        }
    }

    std::cout << "error finding lua boolean" << std::endl;
    return false;
}

std::string LuaGetTableString(const char* tableName, const char* tableKey, const char* filePath)
{
    std::string tableValue;
    if (CheckLua(L, luaL_dofile(L, filePath))) {
        lua_getglobal(L, tableName); // get the table (on top of stack)
        if (lua_istable(L, -1)) {
            lua_pushstring(L, tableKey);      // push table key value on top of stack
            lua_gettable(L, -2);              // table is now in -2 position on stack
            tableValue = lua_tostring(L, -1); // get table value on top of stack
            lua_pop(L, 1);                    // pop the table from the stack
            return tableValue;
        }
    }

    return std::string("null");
}

int LuaGetTableInt(const char* tableName, const char* tableKey, const char* filePath)
{
    int tableValue;
    if (CheckLua(L, luaL_dofile(L, filePath))) {
        lua_getglobal(L, tableName); // get the table (on top of stack)
        if (lua_istable(L, -1)) {
            lua_pushstring(L, tableKey);       // push table key value on top of stack
            lua_gettable(L, -2);               // table is now in -2 position on stack
            tableValue = lua_tointeger(L, -1); // get table value on top of stack
            lua_pop(L, 1);                     // pop the table from the stack
            return tableValue;
        }
    }

    return 0;
}

std::vector<Vector3> LuaGetAtlasVals(const char* tableName, const char* filePath)
{
    std::vector<Vector3> dataStorage;
    if (CheckLua(L, luaL_dofile(L, filePath))) {
        lua_getglobal(L, tableName);
        lua_pushnil(L);

        // Iterate over the table
        while (lua_next(L, -2) != 0) {
            // Key is at index -2 and value is at index -1
            // Print the key

            // Assume the value is a table, traverse the inner table
            if (lua_istable(L, -1)) {
                Vector3 data; // (id = data.z, x = data.y, y = data.x)

                lua_getfield(L, -1, "id");
                data.z = lua_tointeger(L, -1);
                lua_pop(L, 1);

                lua_getfield(L, -1, "x");
                data.x = lua_tointeger(L, -1);
                lua_pop(L, 1);

                lua_getfield(L, -1, "y");
                data.y = lua_tointeger(L, -1);
                lua_pop(L, 1);

                dataStorage.push_back(data);
            }
            lua_pop(L, 1); // Remove value, keep key for next iteration
        }

        // Remove the table from the stack
        lua_pop(L, 1);
    }

    return dataStorage;
}

bool CheckLua(lua_State* L, int response)
{
    if (response != LUA_OK) {
        std::string errormsg = lua_tostring(L, -1);

        std::cout << "LUA: Error: " << errormsg << std::endl;
        return false;
    }

    return true;
}
