#include "sprite_batch.h"
#include "sprite.h"
#include "cvector.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

static cvector_vector_type(SpriteRegionItem) items;

// Function: add
// Arguments:
//   slice: table
//   rect : table
     int sprite_batch_add(lua_State * L)
{
    if (lua_gettop(L) != 2) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }
    // get 'slice' table
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "index");
    int index = luaL_checkinteger(L, -1);

    return 0;
}
