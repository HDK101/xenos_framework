#include "sprite.h"

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <string.h>

#include "config.h"

#include "lua/lauxlib.h"
#include "lua/lualib.h"

#include "cvector.h"
#include "sds.h"

#define MAX_SPRITES 100

static ALLEGRO_BITMAP *sprites[MAX_SPRITES];
static int sprites_size_width[MAX_SPRITES];
static int sprites_size_height[MAX_SPRITES];
static sds sprites_name[MAX_SPRITES];
static int sprites_id[MAX_SPRITES];
static int sprites_count = 0;

static cvector_vector_type(int) sprites_unused_indexes = NULL;

// Internal function
// Function: load_sprite_at_index
// Arguments:
//   file_name: const char*
//   index: int
     static void load_sprite_at_index(const char *file_name, const int index)
{
    sprites[index] = al_load_bitmap(file_name);
    assert(sprites[index] != NULL);
    sprites_size_width[index] = al_get_bitmap_width(sprites[index]);
    sprites_size_height[index] = al_get_bitmap_height(sprites[index]);
    sprites_id[index]++;
    SPRITE_DEBUG_PRINT("File name: %s\n", file_name);
}

// Internal function
// Function: load_sprite
// Arguments:
//   file_name: const char*
static int load_sprite(const char *file_name)
{
    if (cvector_size(sprites_unused_indexes) == 0) {
        load_sprite_at_index(file_name, sprites_count);
        return sprites_count++;
    } else {
        int *last_index = cvector_end(sprites_unused_indexes);

        cvector_pop_back(sprites_unused_indexes);
        load_sprite_at_index(file_name, *last_index);
    }
}

// Internal function
// Function: set_sprite_name
// Arguments:
//   file_name: const char*
static void set_sprite_name(const int index, const char *file_name)
{
    if (sprites_name[index] != NULL)
        sdsfree(sprites_name[index]);
    sprites_name[index] = sdsnew(file_name);
    assert(sprites_name != NULL);
}

// Lua function
// Function: load_sprite
// Arguments:
//   file_name: string
int sprite_lua_load_sprite(lua_State * L)
{
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }

    const char *file_name = luaL_checkstring(L, 1);
    int sprite_index = load_sprite(file_name);

    SPRITE_DEBUG_PRINT("Sprite index: %d\n", sprite_index);
    lua_newtable(L);
    lua_pushinteger(L, sprite_index);
    lua_setfield(L, -2, "index");
    lua_pushstring(L, file_name);
    lua_setfield(L, -2, "file_name");
    lua_pushinteger(L, sprites_id[sprite_index]);
    lua_setfield(L, -2, "id");
    set_sprite_name(sprite_index, file_name);
    return 1;
}

// Lua function
// Function: load_sprite
// Arguments:
//   sprite: table 
int sprite_lua_destroy_sprite(lua_State * L)
{
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }

    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "index");

    int index = lua_tointeger(L, 1);

    if (sprites[index] != NULL)
        al_destroy_bitmap(sprites[index]);
    sprites[index] = NULL;
    cvector_push_back(sprites_unused_indexes, index);

    int *it = NULL;

    for (it = cvector_begin(sprites_unused_indexes);
         it != cvector_end(sprites_unused_indexes); ++it) {
        SPRITE_DEBUG_PRINT("Unused index: %d\n", *it);
    }
    return 0;
}

// Function: draw_sprite
// Arguments:
//   sprite : table
//   rect : table
int sprite_lua_draw_sprite(lua_State * L)
{
    if (lua_gettop(L) != 2) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 2 arguments.\n");
        return lua_error(L);
    }
    // get 'sprite' table
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "index");
    int index = luaL_checkinteger(L, -1);

    SPRITE_DEBUG_PRINT("Sprite index: %d\n", index);

    lua_getfield(L, 1, "file_name");
    const char *coming_file_name = luaL_checkstring(L, -1);

    if (strcmp(sprites_name[index], coming_file_name) != 0) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr,
                "Different file names detected, could not render sprite.\n");
        fprintf(stderr, "expected: %s got: %s.\n", sprites_name[index],
                coming_file_name);
        return lua_error(L);
    }

    lua_getfield(L, 1, "id");
    const int id = luaL_checkinteger(L, -1);

    if (id != sprites_id[index]) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Different IDs detected, could not render sprite.\n");
        fprintf(stderr, "expected: %d got: %d.\n", sprites_id[index], id);
        return lua_error(L);
    }

    if (sprites[index] == NULL) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Empty bitmap pointer.\n");
        return lua_error(L);
    }
    // get 'rect' table
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 2, "x");
    double x = luaL_checknumber(L, -1);

    lua_getfield(L, 2, "y");
    double y = luaL_checknumber(L, -1);

    lua_getfield(L, 2, "angle");
    double angle = luaL_checknumber(L, -1);

    SPRITE_DEBUG_PRINT("X: %lf Y: %lf\n", x, y);
    SPRITE_DEBUG_PRINT("Angle: %lf\n", angle);
    al_draw_rotated_bitmap(sprites[index], sprites_size_width[index] / 2,
                           sprites_size_height[index] / 2, x, y, angle, 0);
    return 0;
}

int sprite_lua_init(lua_State * L)
{
    lua_newtable(L);

    lua_pushcfunction(L, sprite_lua_load_sprite);
    lua_setfield(L, -2, "load_file");

    lua_pushcfunction(L, sprite_lua_draw_sprite);
    lua_setfield(L, -2, "draw");

    lua_pushcfunction(L, sprite_lua_destroy_sprite);
    lua_setfield(L, -2, "destroy");

    lua_setglobal(L, "sprite");

    for (int i = 0; i < MAX_SPRITES; i++) {
        sprites_id[i] = 0;
    }
}
