#ifndef CONFIG_H

// The following macros that ends with _DEBUG can have the following values:
//    0: false
//    1: true

#define SPRITE_DEBUG 0
#define KEYBOARD_DEBUG 1
#define SOUND_DEBUG 1

#if SPRITE_DEBUG == 1
#define SPRITE_DEBUG_PRINT(...) do{ fprintf( stderr, __VA_ARGS__ ); } while( false )
#else
#define SPRITE_DEBUG_PRINT(...) do{ } while ( false )
#endif

#if KEYBOARD_DEBUG == 1
#define KEYBOARD_DEBUG_PRINT(...) do{ fprintf( stderr, __VA_ARGS__ ); } while( false )
#else
#define KEYBOARD_DEBUG_PRINT(...) do{ } while ( false )
#endif

#if SOUND_DEBUG == 1
#define SOUND_DEBUG_PRINT(...) do{ fprintf( stderr, __VA_ARGS__ ); } while( false )
#else
#define SOUND_DEBUG_PRINT(...) do{ } while ( false )
#endif

#endif
