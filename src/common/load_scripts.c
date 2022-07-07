#include "load_scripts.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

void load_scripts(lua_State *L) {
    luaL_loadfile(L, "src/scripts/save/save.lua");
    lua_pcall(L, 0, 0, 0);
}
