#include "load_scripts.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void load_scripts(lua_State * L)
{
    luaL_loadfile(L, "src/scripts/save/save.lua");
    lua_pcall(L, 0, 0, 0);
}
