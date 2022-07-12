#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "lua/lua.h"

void sprite_clear(void);
void sprite_check(lua_State * L, int id, int index, const char *file_name);
ALLEGRO_BITMAP *sprite_get_bitmap(int index);

int sprite_lua_load_sprite(lua_State *L);
int sprite_lua_draw_sprite(lua_State *L);
int sprite_lua_init(lua_State *L);

#endif
