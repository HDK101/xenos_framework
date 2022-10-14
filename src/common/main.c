#include <stdio.h>
#include <allegro5/allegro5.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "allegro_init.h"
#include "sprite.h"
#include "sprite_batch.h"
#include "keyboard.h"
#include "mouse.h"
#include "sound.h"
#include "music.h"
#include "json.h"

#include "cJSON.h"

#include "lfs/lfs.h"

#include "load_scripts.h"

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

void game_loop(double *delta_time)
{
    lua_getglobal(Lstate, "process");
    lua_pushnumber(Lstate, *delta_time);
    int process_code = lua_pcall(Lstate, 1, 0, 0);

    if (process_code == 2) {
        const char *str = lua_tostring(Lstate, -1);

        fputs(str, stderr);
        exit(1);
    }
    lua_getglobal(Lstate, "draw");
    int draw_code = lua_pcall(Lstate, 0, 0, 0);

    if (draw_code == 2) {
        const char *str = lua_tostring(Lstate, -1);

        fputs(str, stderr);
        exit(1);
    }
}

int main()
{
    allegro_init(640, 480);
    allegro_set_game_loop(game_loop);

    Lstate = luaL_newstate();

    luaL_openlibs(Lstate);

    luaopen_lfs(Lstate);

    lua_newtable(Lstate);
    lua_setglobal(Lstate, "xenos");

    sprite_lua_init(Lstate);

    keyboard_init();
    keyboard_lua_init(Lstate);

    mouse_init();
    mouse_lua_init(Lstate);

    sound_init();
    sound_lua_init(Lstate);

    music_init();
    music_lua_init(Lstate);

    json_lua_init(Lstate);

    sprite_batch_lua_init(Lstate);

    load_scripts(Lstate);

    luaL_loadfile(Lstate, "test.lua");
    int code = lua_pcall(Lstate, 0, 0, 0);

    if (code == 2) {
        fputs(lua_tostring(Lstate, -1), stderr);
        return 1;
    }

    allegro_game_loop();

    sprite_clear();

    music_destroy();
    allegro_destroy();

    lua_close(Lstate);
    return 0;
}
