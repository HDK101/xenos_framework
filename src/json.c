#include <stdio.h>
#include <stdbool.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

#include "cJSON.h"
#include "cvector.h"

cJSON *navigate_stringify(lua_State *L);
cJSON *navigate_stringify_array(lua_State *L);
cJSON *navigate_stringify_object(lua_State *L);

static cvector_vector_type(int) array_sizes = NULL;

static void set_to_table(lua_State *L, const char *name) {
  int *index = (cvector_end(array_sizes) - 1);
  if (*index > 0) {
    lua_rawseti(L, -2, *index);
    *index += 1;
  }
  else if (*index == -1) {
    if (name) lua_setfield(L, -2, name);
  }
}

void navigate_parse(lua_State *L, cJSON *json) {
    if (json->type == cJSON_String) {
      lua_pushstring(L, json->valuestring);
      set_to_table(L, json->string);
    }
    else if (json->type == cJSON_Number) {
      lua_pushnumber(L, json->valuedouble);
      set_to_table(L, json->string);
    }
    else if (json->type == cJSON_NULL) {
      lua_pushnil(L);
      set_to_table(L, json->string);
    }
    else if (json->type == cJSON_False) {
      lua_pushboolean(L, false);
      set_to_table(L, json->string);
    }
    else if (json->type == cJSON_True) {
      lua_pushboolean(L, true);
      set_to_table(L, json->string);
    }
    else if (json->type == cJSON_Array) {
      lua_newtable(L);
      cvector_push_back(array_sizes, 1);
    }
    else if (json->type == cJSON_Object) {
      lua_newtable(L);
      cvector_push_back(array_sizes, -1);
    }
  
  if (json->child == NULL) return;
  cJSON *next = json->child;
  while (next != NULL) {
    navigate_parse(L, next);
    next = next->next;
  }
    
    if ((json->type == cJSON_Object || json->type == cJSON_Array)) {
      cvector_pop_back(array_sizes);
      set_to_table(L, json->string);
    }
}

static int json_lua_parse(lua_State *L) {
  if (lua_gettop(L) != 1) {
      fprintf(stderr, "\n--ERROR--\n");
      fprintf(stderr, "Expected exactly 1 argument.\n");
      return lua_error(L);
  }
  
  luaL_checktype(L, 1, LUA_TSTRING);

  cJSON *json = cJSON_Parse(lua_tostring(L, 1));
  if (json == NULL) {
      fprintf(stderr, "\n--ERROR--\n");
      fprintf(stderr, "Invalid JSON.\n");
      return lua_error(L);
  }

  navigate_parse(L, json);
  return 1;
}


cJSON *navigate_stringify(lua_State *L) {
  lua_Unsigned size = lua_rawlen(L, -1);
  if (size == 0) return navigate_stringify_object(L);
  else if (size > 0) return navigate_stringify_array(L);
  return NULL;
}

cJSON *navigate_stringify_array(lua_State *L) {
    cJSON *json = cJSON_CreateArray();
    
    lua_Unsigned size = lua_rawlen(L, -1);

    for (lua_Unsigned i = 1; i <= size; i++) {
      lua_rawgeti(L, 1, i);
      if (lua_isnumber(L, -1)) {
          double value = lua_tonumber(L, -1);
          cJSON *number = cJSON_CreateNumber(value);
          cJSON_AddItemToArray(json, number);
      }
      else if (lua_isstring(L, -1)) {
          const char *value = lua_tostring(L, -1);
          cJSON *string = cJSON_CreateString(value);
          cJSON_AddItemToArray(json, string);
      }
      else if (lua_isboolean(L, -1)) {
          const int value = lua_toboolean(L, -1);
          cJSON *boolean = cJSON_CreateBool(value);
          cJSON_AddItemToArray(json, boolean);
      }
      else if (lua_istable(L , -1)) {
          cJSON *to_add = navigate_stringify(L);
          if (to_add != NULL) cJSON_AddItemToArray(json, to_add);
      }
      lua_pop(L, 1);
    }

    return json;
}

cJSON *navigate_stringify_object(lua_State *L) {
    cJSON *json = cJSON_CreateObject();

    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        if (lua_isstring(L, -2) && lua_isnumber(L, -1)) {
            const char *key = lua_tostring(L, -2);
            double value = lua_tonumber(L, -1);
            cJSON *number = cJSON_CreateNumber(value);
            cJSON_AddItemToObject(json, key, number);
        }
        else if (lua_isstring(L, -2) && lua_isstring(L, -1)) {
            const char *key = lua_tostring(L, -2);
            const char *value = lua_tostring(L, -1);
            cJSON *string = cJSON_CreateString(value);
            cJSON_AddItemToObject(json, key, string);
        }
        else if (lua_isstring(L, -2) && lua_isboolean(L, -1)) {
            const char *key = lua_tostring(L, -2);
            const int value = lua_toboolean(L, -1);
            cJSON *boolean = cJSON_CreateBool(value);
            cJSON_AddItemToObject(json, key, boolean);
        }
        else if (lua_isstring(L, -2) && lua_istable(L , -1)) {
            const char *key = lua_tostring(L, -2);
            cJSON *to_add = navigate_stringify(L);
            if (to_add != NULL) cJSON_AddItemToObject(json, key, to_add);
        }
        lua_pop(L, 1);
    }

    return json;
}

static int json_lua_stringify(lua_State *L) {
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }
    
    
    // get table
    luaL_checktype(L, 1, LUA_TTABLE);

    cJSON *json = navigate_stringify(L);
    lua_pushstring(L, cJSON_Print(json));

    return 1;
}

void json_lua_init(lua_State *L) {
    lua_getglobal(L, "xenos");
    lua_newtable(L);
    
    lua_pushcfunction(L, json_lua_stringify);
    lua_setfield(L, -2, "stringify");

    lua_pushcfunction(L, json_lua_parse);
    lua_setfield(L, -2, "parse");
    
    lua_setfield(L, -2, "json");
}
