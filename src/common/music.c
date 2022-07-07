#include "music.h"

#include <assert.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "cvector.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

#define MAX_STREAMS 100

static ALLEGRO_AUDIO_STREAM* current_music = NULL;
static ALLEGRO_AUDIO_STREAM *streams[MAX_STREAMS];
static int streams_id[MAX_STREAMS];
static int streams_count = 0;
static cvector_vector_type(int) streams_unused_indexes = NULL;

static void load_stream_at_index(const char *file_name, const int index) {
    streams[index] = al_load_audio_stream(file_name, 2, 2048);
    assert(streams[index] != NULL);
    streams_id[index]++;
}

static int load_stream(const char *file_name) {
    if (cvector_size(streams_unused_indexes) > 0) {
        int end = cvector_end(streams_unused_indexes);
        cvector_pop_back(streams_unused_indexes);
        load_stream_at_index(file_name, streams_count);
        return end;
    }
    else {
        load_stream_at_index(file_name, streams_count);
        return streams_count++;
    }
}


static int music_lua_load_stream(lua_State *L) {
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }
    const char *file_name = luaL_checkstring(L, -1);
    const int index = load_stream(file_name);

    lua_newtable(L);

    lua_pushinteger(L, index);
    lua_setfield(L, -2, "index");

    lua_pushinteger(L, streams_id[index]);
    lua_setfield(L, -2, "id");

    return 1;
}

static int music_lua_play(lua_State *L) {
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }

    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "index");
    int index = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "id");
    int id = luaL_checkinteger(L, -1);

    if (id != streams_id[index]) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Different IDs detected, could not play stream.\n");
        fprintf(stderr, "expected: %d got: %d.\n", streams_id[index], id);
        return lua_error(L);
    }

    if (current_music != NULL) {
        al_detach_audio_stream(current_music);
    }

    current_music = streams[index];
    al_set_audio_stream_playmode(current_music, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(current_music, al_get_default_mixer());
    return 0;
}

int music_lua_init(lua_State *L) {
    lua_getglobal(L, "xenos");
    lua_newtable(L);

    lua_pushcfunction(L, music_lua_load_stream);
    lua_setfield(L, -2, "load_stream");

    lua_pushcfunction(L, music_lua_play);
    lua_setfield(L, -2, "play");

    lua_setfield(L, -2, "music");
    return 0;
}

int music_init(void) {
    for (int i = 0; i < MAX_STREAMS; i++) {
        streams_id[i] = 0;
        streams[i] = NULL;
    }
    return 0;
}

int music_destroy(void) {
    for (int i = 0; i < MAX_STREAMS; i++) {
        if (streams[i] != NULL) al_destroy_audio_stream(streams[i]);
    }
    return 0;
}

