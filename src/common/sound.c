#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <assert.h>

#include "config.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

#include "cvector.h"

#define MAX_SAMPLES 100

static ALLEGRO_SAMPLE *samples[MAX_SAMPLES];
static int samples_id[MAX_SAMPLES];
static int samples_count = 0;

static cvector_vector_type(int) samples_unused_indexes = NULL;

// Internal function
// Function: load_sample_at_index
// Arguments:
//   file_name: const char*
//   index: int
static void load_sample_at_index(const char *file_name, const int index) {
    SOUND_DEBUG_PRINT("Loading sample: %s\n", file_name);
    samples[index] = al_load_sample(file_name);
    assert(samples[index] != NULL);
    samples_id[index]++;
}

// Internal function
// Function: load_sample
// Arguments:
//   file_name: const char*
static int load_sample(const char *file_name) {
    if (cvector_size(samples_unused_indexes) > 0) {
        int end = cvector_end(samples_unused_indexes);
        cvector_pop_back(samples_unused_indexes);
        load_sample_at_index(file_name, end);
        return end;
    } else {
        load_sample_at_index(file_name, samples_count);
        return samples_count++;
    }
}

// Lua function
// Function: load_sample
// Arguments:
//   file_name: string
static int sound_lua_load_sample(lua_State *L) {
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }
    
    const char *file_name = luaL_checkstring(L, 1);
    const int index = load_sample(file_name);

    lua_newtable(L);
    
    lua_pushinteger(L, index);
    lua_setfield(L, -2, "index");
    
    lua_pushinteger(L, ++samples_id[index]);
    lua_setfield(L, -2, "id");
    
    return 1;
}

// Lua function
// Function: play
// Arguments:
//   sample: table
static int sound_lua_play_sample(lua_State *L) {
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }

    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "index");
    int index = luaL_checkinteger(L, -1);
    SOUND_DEBUG_PRINT("Playing sample index: %d\n", index);
    
    lua_getfield(L, 1, "id");
    int id = luaL_checkinteger(L, -1);

    if (id != samples_id[index]) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Different IDs detected, could not play sample.\n");
        fprintf(stderr, "expected: %d got: %d.\n", samples_id[index], id);
        return lua_error(L);
    }

    al_play_sample(samples[index], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    
    return 0;
}

// Global function
// Function: sound_init
// Arguments: void
int sound_init(void) {
    for (int i = 0; i < MAX_SAMPLES; i++) {
        samples_id[i] = 0;
    }
}

// Global function
// Function: sound_lua_init
// Arguments: 
//    L: *lua_State
int sound_lua_init(lua_State *L) {
    lua_getglobal(L, "xenos");
    lua_newtable(L);

    lua_pushcfunction(L, sound_lua_load_sample);
    lua_setfield(L, -2, "load_sample");

    lua_pushcfunction(L, sound_lua_play_sample);
    lua_setfield(L, -2, "play");

    lua_setglobal(L, "sound");
    
    lua_setfield(L, -2, "sound");
}
