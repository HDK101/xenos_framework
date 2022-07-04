#ifndef JSON_H
#define JSON_H

#include "cJSON.h"

#include "lua/lua.h"

void create_json(lua_State *L, cJSON *json);
void navigate(lua_State *L, cJSON *json);
int to_json(lua_State *L);
void json_lua_init(lua_State *L);
#endif /* ifndef JSON_H
 */
