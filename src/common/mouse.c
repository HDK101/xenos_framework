#include "mouse.h"

#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

static bool mouse_left_pressed = 0;
static bool mouse_right_pressed = 0;

static bool mouse_left_released = 0;
static bool mouse_right_released = 0;

static int mouse_x = 0;
static int mouse_y = 0;

// Lua function
// Function: press
// Arguments:
//    keycode: integer
static int mouse_lua_press(lua_State *L) {
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }
    const int keycode = luaL_checkinteger(L, -1);
    if (keycode == 0) lua_pushboolean(L, mouse_left_pressed);
    else if (keycode == 2) lua_pushboolean(L, mouse_right_pressed);
    return 1;
}

// Lua function
// Function: release
// Arguments:
//    keycode: integer
static int mouse_lua_release(lua_State *L) {
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }
    const int keycode = luaL_checkinteger(L, -1);
    if (keycode == 0) lua_pushboolean(L, mouse_left_released);
    else if (keycode == 2) lua_pushboolean(L, mouse_right_released);
    return 1;
}

// Lua function
// Function: get_position
static int mouse_lua_get_position(lua_State *L) {
    lua_newtable(L);
    lua_pushnumber(L, mouse_x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, mouse_y);
    lua_setfield(L, -2, "y");
    return 1;
}

void mouse_event_down(ALLEGRO_EVENT *event) {
    mouse_left_pressed = event->mouse.button & 1;
    mouse_right_pressed = event->mouse.button & 2;
}

void mouse_event_up(ALLEGRO_EVENT *event) {
    mouse_left_released = event->mouse.button & 1;
    mouse_right_released = event->mouse.button & 2;
}

void mouse_event_move(ALLEGRO_EVENT *event) {
    mouse_x = event->mouse.x;
    mouse_y = event->mouse.y;
}

void mouse_clear(void) {
    mouse_left_pressed = false;
    mouse_right_pressed = false;

    mouse_left_released = false;
    mouse_right_released = false;
}

void mouse_lua_init(lua_State *L) {
    lua_getglobal(L, "xenos");
    lua_newtable(L);

    lua_pushcfunction(L, mouse_lua_press);
    lua_setfield(L, -2, "press");

    lua_pushcfunction(L, mouse_lua_release);
    lua_setfield(L, -2, "release");

    lua_pushcfunction(L, mouse_lua_get_position);
    lua_setfield(L, -2, "get_position");

    lua_setfield(L, -2, "mouse");
}

void mouse_init(void) {
    printf("Mouse init!\n");
}

