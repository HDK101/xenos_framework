#include <stdio.h>
#include <allegro5/allegro5.h>

#include "allegro_init.h"
#include "sprite.h"
#include "keyboard.h"
#include "mouse.h"
#include "sound.h"
#include "music.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

static lua_State *Lstate = NULL;

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

void game_loop(double *delta_time)
{
    lua_getglobal(Lstate, "process");
    lua_pushnumber(Lstate, *delta_time);
    lua_pcall(Lstate, 1, 0, 0);
    lua_getglobal(Lstate, "draw");
    lua_pcall(Lstate, 0, 0, 0);
}

int main()
{
    allegro_init();
    allegro_set_game_loop(game_loop);

    Lstate = luaL_newstate();

    luaL_openlibs(Lstate);

    sprite_lua_init(Lstate);

    keyboard_init();
    keyboard_lua_init(Lstate);

    mouse_init();
    mouse_lua_init(Lstate);

    sound_init();
    sound_lua_init(Lstate);

    music_init();
    music_lua_init(Lstate);

    luaL_loadfile(Lstate, "test.lua");
    lua_pcall(Lstate, 0, 0, 0);
    //lua_call(Lstate, 0, 0);

    allegro_game_loop();

    music_destroy();
    allegro_destroy();

    lua_close(Lstate);
    return 0;
}
