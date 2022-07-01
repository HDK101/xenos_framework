#include <stdio.h>
#include <stdbool.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

#include "cJSON.h"
#include "cvector.h"

static void stackDump(lua_State * L)
{
    int i;
    int top = lua_gettop(L);

    for (i = 1; i <= top; i++) {        /* repeat for each level */
        int t = lua_type(L, i);

        switch (t) {
            case LUA_TSTRING:  /* strings */
                printf("`%s'", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN: /* booleans */
                printf(lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:  /* numbers */
                printf("%g", lua_tonumber(L, i));
                break;
            default:           /* other values */
                printf("%s", lua_typename(L, t));
                break;
        }
        printf("  ");           /* put a separator */
        printf("\n");           /* end the listing */
    }
}

static cvector_vector_type(int) array_sizes = NULL;

static int json_lua_parse(lua_State *L) {
  const cJSON *json = cJSON_Parse("{ \"name\": \"walter\" }");
  return 0;
}

static void set_to_table(lua_State *L, const char *name) {
  int *iindex = (cvector_end(array_sizes) - 1);
  printf("INDEX: %d\n", *iindex);
  if (*iindex > 0) {
    lua_rawseti(L, -2, *iindex);
    *iindex += 1;
  }
  else if (*iindex == -1) {
    if (name) lua_setfield(L, -2, name);
  }
}

void navigate(lua_State *L, cJSON *json) {
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
      printf("Array start!\n");
    }
    else if (json->type == cJSON_Object) {
      lua_newtable(L);
      cvector_push_back(array_sizes, -1);
      printf("Object start!\n");
    }
  
  if (json->child == NULL) return;
  cJSON *next = json->child;
  while (next != NULL) {
    navigate(L, next);
    next = next->next;
  }
    
    if ((json->type == cJSON_Object || json->type == cJSON_Array)) {
      cvector_pop_back(array_sizes);
      set_to_table(L, json->string);
      if (json->type == cJSON_Object) printf("Object end!\n");
      if (json->type == cJSON_Array) printf("Array end!\n");
    }
}

void create_json(lua_State *L, cJSON *json) {
  navigate(L, json);
  lua_setglobal(L, "json_test");
}

