#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "sprite_batch.h"
#include "sprite.h"
#include "cvector.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

static cvector_vector_type(SpriteRegionItem *) items;
     static ALLEGRO_BITMAP *target;

// Function: add
// Arguments:
//   slice: table
//   rect : table
     int sprite_batch_lua_set_target(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "index");
    const int index = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "id");
    const int id = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "file_name");
    const char *coming_file_name = luaL_checkstring(L, -1);

    sprite_check(L, id, index, coming_file_name);

    target = sprite_get_bitmap(index);

    return 0;
}

// Function: add
// Arguments:
//   slice: table
//   rect : table
int sprite_batch_lua_add(lua_State * L)
{
    if (lua_gettop(L) != 2) {
        lua_pushstring(L, "Expected exactly 2 arguments.\n");
        lua_error(L);
    }
    // get 'sprite' table
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "index");
    const int index = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "id");
    const int id = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "file_name");
    const char *coming_file_name = luaL_checkstring(L, -1);

    sprite_check(L, id, index, coming_file_name);

    // get 'rect' table
    luaL_checktype(L, 2, LUA_TTABLE);

    SpriteRegionItem *item = malloc(sizeof(SpriteRegionItem));

    lua_getfield(L, 1, "sh");
    item->sh = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "sw");
    item->sw = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "sx");
    item->sx = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "sy");
    item->sy = luaL_checkinteger(L, -1);

    lua_getfield(L, 2, "x");
    item->x = luaL_checkinteger(L, -1);

    lua_getfield(L, 2, "y");
    item->y = luaL_checkinteger(L, -1);

    cvector_push_back(items, item);

    return 0;
}

int sprite_batch_lua_draw(lua_State * L)
{
    SpriteRegionItem *item = NULL;

    if (items == NULL)
        return 0;

    for (item = cvector_begin(items); item != cvector_end(items); ++item) {
        al_draw_bitmap_region(target, item->sx, item->sy, item->sw, item->sh,
                              item->x, item->y, 0);
    }

    return 0;
}

int sprite_batch_lua_clear(lua_State * L)
{
    if (items == NULL)
        return 0;

    for (size_t i = 0; i < cvector_size(items); i++) {
        free(items[i]);
    }

    cvector_free(items);
    items = NULL;

    return 0;
}

int sprite_batch_lua_init(lua_State * L)
{
    lua_getglobal(L, "xenos");
    lua_newtable(L);

    lua_pushcfunction(L, sprite_batch_lua_add);
    lua_setfield(L, -2, "add");

    lua_pushcfunction(L, sprite_batch_lua_draw);
    lua_setfield(L, -2, "draw");

    lua_pushcfunction(L, sprite_batch_lua_set_target);
    lua_setfield(L, -2, "set_target");

    lua_pushcfunction(L, sprite_batch_lua_clear);
    lua_setfield(L, -2, "clear");

    lua_setfield(L, -2, "sprite_batch");
    return 0;
}
