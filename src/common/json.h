#ifndef JSON_H
#define JSON_H

#include "cJSON.h"

#include <lua.h>

void create_json(lua_State *L, cJSON *json);
cJSON *navigate_stringify_array(lua_State *L);
cJSON *navigate_stringify_object(lua_State *L);
cJSON *navigate_stringify(lua_State *L);
int to_json(lua_State *L);
void json_lua_init(lua_State *L);
#endif /* ifndef JSON_H
 */
