#ifndef MUSIC_H
#define MUSIC_H

#include <lua.h>

int music_init(void);
int music_destroy(void);
int music_lua_init(lua_State *L);

#endif
