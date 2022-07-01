#ifndef JSON_H
#define JSON_H

#include "cJSON.h"

void create_json(lua_State *L, cJSON *json);
void navigate(lua_State *L, cJSON *json);
#endif /* ifndef JSON_H
 */
