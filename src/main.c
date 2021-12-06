#include <stdio.h>
#include <allegro5/allegro5.h>

#include "allegro_init.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

static void stackDump(lua_State * L)
{
    int i;
    int top = lua_gettop(L);

    for (i = 1; i <= top; i++) {        /* repeat for each level */
        int t = lua_type(L, i);

        switch (t) {
            case LUA_TSTRING:  /* strings */
                printf("`%s'", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN: /* booleans */
                printf(lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:  /* numbers */
                printf("%g", lua_tonumber(L, i));
                break;
            default:           /* other values */
                printf("%s", lua_typename(L, t));
                break;
        }
        printf("  ");           /* put a separator */
        printf("\n");           /* end the listing */
    }
}

static int functiontest(lua_State * L)
{
    printf("Hello world!\n");
    lua_pushnumber(L, 565);
    return 1;
}

void game_loop(void) {
    printf("Game loop!");
}

int main()
{
    allegro_init();
    allegro_set_game_loop(game_loop);

    lua_State *L = luaL_newstate();

    luaL_openlibs(L);

    lua_newtable(L);
    lua_pushnumber(L, 5.456);
    lua_setfield(L, -2, "numba");
    lua_setglobal(L, "bigchungus");

    lua_pushcfunction(L, functiontest);
    lua_setglobal(L, "functiontest");

    luaL_loadfile(L, "test.lua");
    lua_pcall(L, 0, 0, 0);

    lua_close(L);
    return 0;
}
