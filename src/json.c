#include <stdio.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

#include "cJSON.h";

static int json_lua_parse(lua_State *L) {
  const cJSON *json = cJSON_Parse("{ \"name\": \"walter\" }");
}

void create_json(lua_State *L, cJSON *json) {
  lua_newtable(L);
  navigate(L, json);
}

void navigate(lua_State *L, cJSON *json) {
  if (json->string != NULL) {
    if (json->type == cJSON_String) {
      lua_pushstring(L, json->valuestring);
      lua_setfield(L, -2, json->string);
    }
    else if (json->type == cJSON_Object) {
      lua_newtable(L);
    }
  }
  
  if (json->child == NULL) return;
  const cJSON *child = json->child;
  cJSON *next = child;
  while (next != NULL) {
    navigate(L, next);
    next = next->next;
  }
    
  if (json->string != NULL && json->type == cJSON_Object) {
    lua_setfield(L, -2, json->string);
  }
}
