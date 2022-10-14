#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <allegro5/allegro.h>
#include <lua.h>

void keyboard_lua_init(lua_State *L);
void keyboard_init(void);
void keyboard_event_down(ALLEGRO_EVENT *event);
void keyboard_event_up(ALLEGRO_EVENT *event);
void keyboard_clear(void);

#endif
