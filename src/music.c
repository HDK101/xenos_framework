#include "music.h"

#include <assert.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

static ALLEGRO_AUDIO_STREAM* current_music = NULL;

int music_lua_load_stream(lua_State *L) {
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }
    const char *file_name = luaL_checkstring(L, -1);
    if (current_music != NULL) {
        al_detach_audio_stream(current_music);
        al_destroy_audio_stream(current_music);
    }
    current_music = al_load_audio_stream(file_name, 2, 2048);
}

int music_lua_play(lua_State *L) {
    al_set_audio_stream_playmode(current_music, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(current_music, al_get_default_mixer());
}

int music_lua_init(lua_State *L) {
    lua_newtable(L);

    lua_pushcfunction(L, music_lua_load_stream);
    lua_setfield(L, -2, "load_stream");
    
    lua_pushcfunction(L, music_lua_play);
    lua_setfield(L, -2, "play");

    lua_setglobal(L, "music");
}

int music_init(void) {
    return 0;
}

