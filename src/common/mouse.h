#ifndef MOUSE_H
#define MOUSE_H

#include <allegro5/allegro.h>
#include <lua.h>

void mouse_event_down(ALLEGRO_EVENT *event);
void mouse_event_up(ALLEGRO_EVENT *event);
void mouse_event_move(ALLEGRO_EVENT *event);
void mouse_clear(void);

void mouse_lua_init(lua_State *L);
void mouse_init(void);

#endif
