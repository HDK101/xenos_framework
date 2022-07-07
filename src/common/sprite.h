#ifndef SPRITE_H
#define SPRITE_H

#include "lua/lua.h"

int sprite_lua_load_sprite(lua_State *L);
int sprite_lua_draw_sprite(lua_State *L);
int sprite_lua_init(lua_State *L);

#endif
