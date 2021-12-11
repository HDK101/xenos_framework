#include "keyboard.h"

#include <allegro5/allegro.h>
#include <stdio.h>
#include <stdbool.h>

#include "sds.h"

#include "cvector.h"

#include "config.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

static bool keys_pressed[ALLEGRO_KEY_MAX];
static cvector_vector_type(int) pressed_keys_indexes = NULL;

static bool keys_released[ALLEGRO_KEY_MAX];
static cvector_vector_type(int) released_keys_indexes = NULL;

// Lua function
// Function: press
// Arguments: 
//    keycode: integer
static int keyboard_lua_press(lua_State *L) {
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }
    const int keycode = luaL_checkinteger(L, -1);
    lua_pushboolean(L, keys_pressed[keycode]);
    return 1;
}

// Lua function
// Function: release
// Arguments: 
//    keycode: integer
static int keyboard_lua_release(lua_State *L) {
    if (lua_gettop(L) != 1) {
        fprintf(stderr, "\n--ERROR--\n");
        fprintf(stderr, "Expected exactly 1 argument.\n");
        return lua_error(L);
    }
    const int keycode = luaL_checkinteger(L, -1);
    lua_pushboolean(L, keys_released[keycode]);
    return 1;
}

// Global function
// Function: keyboard_lua_add_key
// Arguments: 
//    L: lua_State*
//    field: char*
//    keycode: int
static void keyboard_lua_add_key(lua_State *L, const char *field, const int keycode) {
    lua_pushinteger(L, keycode);
    lua_setfield(L, -2, field);
}

// Global function
// Function: keyboard_lua_init
// Arguments: 
//    L: lua_State*
void keyboard_lua_init(lua_State *L) {
    lua_newtable(L);

    lua_pushcfunction(L, keyboard_lua_press);
    lua_setfield(L, -2, "press");
    
    lua_pushcfunction(L, keyboard_lua_release);
    lua_setfield(L, -2, "release");

    lua_newtable(L);
    keyboard_lua_add_key(L, "A", ALLEGRO_KEY_A);
    keyboard_lua_add_key(L, "B", ALLEGRO_KEY_B);
    keyboard_lua_add_key(L, "C", ALLEGRO_KEY_C);
    keyboard_lua_add_key(L, "D", ALLEGRO_KEY_D);
    keyboard_lua_add_key(L, "E", ALLEGRO_KEY_E);
    keyboard_lua_add_key(L, "F", ALLEGRO_KEY_F);
    keyboard_lua_add_key(L, "G", ALLEGRO_KEY_G);
    keyboard_lua_add_key(L, "H", ALLEGRO_KEY_H);
    keyboard_lua_add_key(L, "I", ALLEGRO_KEY_I);
    keyboard_lua_add_key(L, "J", ALLEGRO_KEY_J);
    keyboard_lua_add_key(L, "K", ALLEGRO_KEY_K);
    keyboard_lua_add_key(L, "L", ALLEGRO_KEY_L);
    keyboard_lua_add_key(L, "M", ALLEGRO_KEY_M);
    keyboard_lua_add_key(L, "N", ALLEGRO_KEY_N);
    keyboard_lua_add_key(L, "O", ALLEGRO_KEY_O);
    keyboard_lua_add_key(L, "P", ALLEGRO_KEY_P);
    keyboard_lua_add_key(L, "Q", ALLEGRO_KEY_Q);
    keyboard_lua_add_key(L, "R", ALLEGRO_KEY_R);
    keyboard_lua_add_key(L, "S", ALLEGRO_KEY_S);
    keyboard_lua_add_key(L, "T", ALLEGRO_KEY_T);
    keyboard_lua_add_key(L, "U", ALLEGRO_KEY_U);
    keyboard_lua_add_key(L, "V", ALLEGRO_KEY_V);
    keyboard_lua_add_key(L, "W", ALLEGRO_KEY_W);
    keyboard_lua_add_key(L, "X", ALLEGRO_KEY_X);
    keyboard_lua_add_key(L, "Y", ALLEGRO_KEY_Y);
    keyboard_lua_add_key(L, "Z", ALLEGRO_KEY_Z);

    char num_string[6];
    for (int i = 0; i < 9; i++) {
        sprintf(num_string, "KEY_%d", i);
        keyboard_lua_add_key(L, num_string, ALLEGRO_KEY_0 + i);
    }

    /*
    keyboard_lua_add_key(L, "ZERO", ALLEGRO_KEY_0);
    keyboard_lua_add_key(L, "ONE", ALLEGRO_KEY_1);
    keyboard_lua_add_key(L, "TWO", ALLEGRO_KEY_2);
    keyboard_lua_add_key(L, "THREE", ALLEGRO_KEY_3);
    keyboard_lua_add_key(L, "FOUR", ALLEGRO_KEY_4);
    keyboard_lua_add_key(L, "FIVE", ALLEGRO_KEY_5);
    keyboard_lua_add_key(L, "SIX", ALLEGRO_KEY_6);
    keyboard_lua_add_key(L, "SEVEN", ALLEGRO_KEY_7);
    keyboard_lua_add_key(L, "EIGHT", ALLEGRO_KEY_8);
    keyboard_lua_add_key(L, "NINE", ALLEGRO_KEY_9);
    */
    
    char pad_string[6];
    for (int i = 0; i < 9; i++) {
        sprintf(pad_string, "PAD_%d", i);
        keyboard_lua_add_key(L, pad_string, ALLEGRO_KEY_PAD_0 + i);
    }
    /*
    keyboard_lua_add_key(L, "PAD_1", ALLEGRO_KEY_PAD_1);
    keyboard_lua_add_key(L, "PAD_2", ALLEGRO_KEY_PAD_2);
    keyboard_lua_add_key(L, "PAD_3", ALLEGRO_KEY_PAD_3);
    keyboard_lua_add_key(L, "PAD_4", ALLEGRO_KEY_PAD_4);
    keyboard_lua_add_key(L, "PAD_5", ALLEGRO_KEY_PAD_5);
    keyboard_lua_add_key(L, "PAD_6", ALLEGRO_KEY_PAD_6);
    keyboard_lua_add_key(L, "PAD_7", ALLEGRO_KEY_PAD_7);
    keyboard_lua_add_key(L, "PAD_8", ALLEGRO_KEY_PAD_8);
    keyboard_lua_add_key(L, "PAD_9", ALLEGRO_KEY_PAD_9);
    */
    
    char f_string[3];
    for (int i = 1; i < 12; i++) {
        sprintf(f_string, "F%d", i);
        keyboard_lua_add_key(L, f_string, ALLEGRO_KEY_F1 + (i - 1));
    }
    
    keyboard_lua_add_key(L, "ESCAPE", ALLEGRO_KEY_ESCAPE);
    keyboard_lua_add_key(L, "TILDE", ALLEGRO_KEY_TILDE);
    keyboard_lua_add_key(L, "MINUS", ALLEGRO_KEY_MINUS);
    keyboard_lua_add_key(L, "EQUALS", ALLEGRO_KEY_EQUALS);
    keyboard_lua_add_key(L, "BACKSPACE", ALLEGRO_KEY_BACKSPACE);
    keyboard_lua_add_key(L, "TAB", ALLEGRO_KEY_TAB);
    keyboard_lua_add_key(L, "OPENBRACE", ALLEGRO_KEY_OPENBRACE);
    keyboard_lua_add_key(L, "CLOSEBRACE", ALLEGRO_KEY_CLOSEBRACE);
    keyboard_lua_add_key(L, "ENTER", ALLEGRO_KEY_ENTER);
    keyboard_lua_add_key(L, "SEMICOLON", ALLEGRO_KEY_SEMICOLON);
    keyboard_lua_add_key(L, "QUOTE", ALLEGRO_KEY_QUOTE);
    keyboard_lua_add_key(L, "BACKSLASH", ALLEGRO_KEY_BACKSLASH);
    keyboard_lua_add_key(L, "BACKSLASH2", ALLEGRO_KEY_BACKSLASH2);
    keyboard_lua_add_key(L, "COMMA", ALLEGRO_KEY_COMMA);
    keyboard_lua_add_key(L, "FULLSTOP", ALLEGRO_KEY_FULLSTOP);
    keyboard_lua_add_key(L, "SLASH", ALLEGRO_KEY_SLASH);
    keyboard_lua_add_key(L, "SPACE", ALLEGRO_KEY_SPACE);
    keyboard_lua_add_key(L, "INSERT", ALLEGRO_KEY_INSERT);
    keyboard_lua_add_key(L, "DELETE", ALLEGRO_KEY_DELETE);
    keyboard_lua_add_key(L, "HOME", ALLEGRO_KEY_HOME);
    keyboard_lua_add_key(L, "END", ALLEGRO_KEY_HOME);
    keyboard_lua_add_key(L, "PGUP", ALLEGRO_KEY_PGUP);
    keyboard_lua_add_key(L, "PGDN", ALLEGRO_KEY_PGDN);
    keyboard_lua_add_key(L, "LEFT", ALLEGRO_KEY_LEFT);
    keyboard_lua_add_key(L, "RIGHT", ALLEGRO_KEY_RIGHT);
    keyboard_lua_add_key(L, "UP", ALLEGRO_KEY_UP);
    keyboard_lua_add_key(L, "DOWN", ALLEGRO_KEY_DOWN);
    keyboard_lua_add_key(L, "LSHIFT", ALLEGRO_KEY_LSHIFT);
    keyboard_lua_add_key(L, "RSHIFT", ALLEGRO_KEY_RSHIFT);
    keyboard_lua_add_key(L, "LCTRL", ALLEGRO_KEY_LCTRL);
    keyboard_lua_add_key(L, "RCTRL", ALLEGRO_KEY_RCTRL);
    keyboard_lua_add_key(L, "ALT", ALLEGRO_KEY_ALT);
    keyboard_lua_add_key(L, "ALTGR", ALLEGRO_KEY_ALTGR);
    keyboard_lua_add_key(L, "LWIN", ALLEGRO_KEY_LWIN);
    keyboard_lua_add_key(L, "RWIN", ALLEGRO_KEY_RWIN);

    lua_setfield(L, -2, "code");
    
    lua_setglobal(L, "keyboard");
}

// Global function
// Function: keyboard_init
// Arguments: void
void keyboard_init(void) {
    for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
        keys_pressed[i] = false;
    }
}

// Global function
// Function: keyboard_event_down
// Arguments: 
//   event: ALLEGRO_EVENT*
void keyboard_event_down(ALLEGRO_EVENT *event) {
    keys_pressed[event->keyboard.keycode] = true;
    cvector_push_back(pressed_keys_indexes, event->keyboard.keycode);
    KEYBOARD_DEBUG_PRINT("KeyCode press: %d\n", event->keyboard.keycode);
}

// Global function
// Function: keyboard_event_up
// Arguments: 
//   event: ALLEGRO_EVENT*
void keyboard_event_up(ALLEGRO_EVENT *event) {
    keys_released[event->keyboard.keycode] = true;
    cvector_push_back(released_keys_indexes, event->keyboard.keycode);
    KEYBOARD_DEBUG_PRINT("KeyCode release: %d\n", event->keyboard.keycode);
}

// Global function
// Function: keyboard_clear
// Arguments: void
void keyboard_clear(void) {
    int *it = NULL;

    //PRESS
    for (it = cvector_begin(pressed_keys_indexes);
         it != cvector_end(pressed_keys_indexes); ++it) {
        keys_pressed[*it] = false;
        KEYBOARD_DEBUG_PRINT("Key press clear: %d\n", *it);
    }
    while(cvector_size(pressed_keys_indexes) > 0) {
        cvector_pop_back(pressed_keys_indexes);
    }
    
    //RELEASE
    for (it = cvector_begin(released_keys_indexes);
         it != cvector_end(released_keys_indexes); ++it) {
        keys_released[*it] = false;
        KEYBOARD_DEBUG_PRINT("Key release clear: %d\n", *it);
    }
    while(cvector_size(released_keys_indexes) > 0) {
        cvector_pop_back(released_keys_indexes);
    }
}

